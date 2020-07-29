#include "Scripts.h"

#include <memory>

void increaseIndex() {
   if (++playerIndex == sessionPlayers) {
      playerIndex = 0;
   }
}

void createPlayerPositions(unsigned short int nPlayers) {
   sessionPlayers = nPlayers;

   float angle = (M_PI * 2.0f) / static_cast<float>(nPlayers);

   // TODO use dynamic index
   maxZ = objects.at(1).getHighestZPoint();

   players.reserve(nPlayers);

   // Use maxY for effective radius
   for (int i = 0; i < nPlayers; ++i) {
      players.emplace_back(Player(40/nPlayers, Float3(8.00f* cosf(i * angle), 9.00f*sinf(i * angle), maxZ+3)));
   }
}

void pollInput(GLFWwindow *window) {
   float speed = 0.01f + ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 0.04f : 0);

   // Funzione per l'input, esempio via tastiera
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   }

   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
      if (sumTimeCardAnimationTime == 0 && !CARD_ANIMATION && IS_GAME_STARTED) {
         if (selectedCardIndex != MAX_SIZE_T_VALUE) {
            cardAnimationTime = 1;

            CARD_ANIMATION = true;
         }
      }
   }

   if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
      if (sumTimeTranslCamera == 0) {
         cameraPlayerPositionTime = 2;
         MENU_TRANSLATION_CAMERA = true;
      }
   }

   if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
      if (IS_GAME_STARTED) {
         if (sumTimeTranslCamera == 0) {
            cameraPlayerPositionTime = 1;

            PLAYER_TRANSLATION_CAMERA = true;
         }
      }
   }

   if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
      DEVELOPER_MODE = !DEVELOPER_MODE;

      if (DEVELOPER_MODE) {
         glfwSetCursorPosCallback(window, cursorPositionCallBack);
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      } else {
         glfwSetCursorPosCallback(window, nullptr);
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
   }

   if (DEVELOPER_MODE) {
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
         Float3 vec(camera.getEye() - camera.getLookAt());
         vec.normalize();
         float tmp = vec.getX();
         vec.setX(-vec.getY());
         vec.setY(tmp);
         vec.setZ(0);
         camera.setEye(camera.getEye() - speed*vec);
         camera.setLookAt(camera.getLookAt() - speed*vec);
      }

      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
         Float3 vec(camera.getEye() - camera.getLookAt());
         vec.normalize();
         float tmp = vec.getX();
         vec.setX(-vec.getY());
         vec.setY(tmp);
         vec.setZ(0);
         camera.setEye(camera.getEye() + speed*vec);
         camera.setLookAt(camera.getLookAt() + speed*vec);
      }

      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
         Float3 vec(camera.getEye() - camera.getLookAt());
         vec.normalize();
         camera.setEye(camera.getEye() + speed*vec);
         camera.setLookAt(camera.getLookAt() + speed*vec);
      }

      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
         Float3 vec(camera.getEye() - camera.getLookAt());
         vec.normalize();
         camera.setEye(camera.getEye() - speed*vec);
         camera.setLookAt(camera.getLookAt() - speed*vec);
      }
   }

   if (CARD_ANIMATION) {
      if (!BUSY_AT_CARD_ANIMATION) {
         BUSY_AT_CARD_ANIMATION = true;

         if (sumTimeCardAnimationTime == 0) {
            cardsOnTable.emplace_back(players.at(playerIndex).getCards().at(selectedCardIndex));
            players.at(playerIndex).getCards().erase(players.at(playerIndex).getCards().begin() + selectedCardIndex);
         }

         double diff = currTime - prevTime;

         if (!cardMovingAnimation) {
            cardMovingAnimation = std::move(std::make_unique<CardMoving>(&cardsOnTable.at(cardsOnTable.size()-1), cardAnimationTime));
         }

         if (sumTimeCardAnimationTime + diff <= cardAnimationTime) {
            sumTimeCardAnimationTime += diff;

            cardMovingAnimation->moveCard(cardMovingAnimation->getCard()->tmp, sumTimeCardAnimationTime);
         } else {
            cardMovingAnimation.reset();
            sumTimeCardAnimationTime = 0;
            CardMoving::heightBias += 0.0025f;

            BUSY_AT_CARD_ANIMATION = false;
            CARD_ANIMATION = false;

            sumTimeTranslCamera = 1;
            PLAYER_TRANSLATION_CAMERA = true;
         }

         BUSY_AT_CARD_ANIMATION = false;
      } else {
         CARD_ANIMATION = false;
      }
   }

   if (PLAYER_TRANSLATION_CAMERA && IS_GAME_STARTED) {
      if (!IS_ANIMATION_BUSY) {
         IS_ANIMATION_BUSY = true;

         double diff = currTime - prevTime;

         if (!cameraRotation || !cameraTranslation2) {
            increaseIndex();

            cameraRotation = std::move(std::make_unique<CameraRotation>(camera.getEye(), players.at(playerIndex).getPosition(), 0));
            cameraTranslation2 = std::move(std::make_unique<CameraTranslation>(camera.getLookAt(), lookAt));
         }

         if (sumTimeTranslCamera + diff <= cameraPlayerPositionTime) {
            sumTimeTranslCamera += diff;

            camera.setEye(cameraRotation->rotateCamera(camera.getEye(), diff));
            camera.setLookAt(cameraTranslation2->translateCamera(sumTimeTranslCamera, cameraPlayerPositionTime));
         } else {
            cameraRotation.reset();
            cameraTranslation2.reset();
            camera.setEye(players.at(playerIndex).getPosition());
            camera.setLookAt(lookAt);
            sumTimeTranslCamera = 0;

            PLAYER_TRANSLATION_CAMERA = false;
         }

         IS_ANIMATION_BUSY = false;
      }
   }

   if (MENU_TRANSLATION_CAMERA && !IS_GAME_STARTED) {
      // TODO fix IS_ANIMATION_BUSY usage (permette una microfrazione di secondo di poter attivare un'altra animazione da un altro tasto)
      if (!IS_ANIMATION_BUSY) {
         IS_ANIMATION_BUSY = true;

         double diff = currTime - prevTime;

         if (!cameraTranslation) {
            // Quando necessario, 0 diventerà l'indice del giocatore
            cameraTranslation = std::move(std::make_unique<CameraTranslation>(camera.getEye(), players.at(0).getPosition()));
            cameraTranslation2 = std::move(std::make_unique<CameraTranslation>(camera.getLookAt(), lookAt));
         }

         if (sumTimeTranslCamera + diff <= cameraPlayerPositionTime) {
            sumTimeTranslCamera += diff;

            camera.setEye(cameraTranslation->translateCamera(sumTimeTranslCamera, cameraPlayerPositionTime));
            camera.setLookAt(cameraTranslation2->translateCamera(sumTimeTranslCamera, cameraPlayerPositionTime));


            blurValue = sumTimeTranslCamera/cameraPlayerPositionTime;
         } else {
            cameraTranslation.reset();
            camera.setEye(players.at(0).getPosition());
            sumTimeTranslCamera = 0;
            blurValue = 1;
            MENU_TRANSLATION_CAMERA = false;
            IS_GAME_STARTED = true;
         }

         IS_ANIMATION_BUSY = false;
      }
   }

   //---------------------------------------------------------------------------------------------------//
}

void cursorPositionCallBack(GLFWwindow *window, double xPos, double yPos) {
   if (DEVELOPER_MODE) {
      float diffX = camera.getSensibility() * (xPos - prevXPos);
      float diffY = camera.getSensibility() * (yPos - prevYPos);

      if (diffX != 0) {
         float angle = atanf(diffX/100.0f);

         Float3 tmp(move((camera.getLookAt() - camera.getEye()).getNormalized()));
         tmp = std::move(Rotation::axisZRotateVertex3(tmp, -angle));

         tmp += camera.getEye();
         camera.setLookAt(tmp);

         prevXPos = xPos;
      }

      if (diffY != 0) {
         float angle = atanf(diffY/100.0f);

         Float3 tmp(move((camera.getLookAt() - camera.getEye()).getNormalized()));

         // TODO fix camera
         tmp = (tmp.getY() < 0 ? std::move(Rotation::axisXRotateVertex3(tmp, angle)) : std::move(Rotation::axisXRotateVertex3(tmp, -angle)))
               + (tmp.getX() > 0 ? std::move(Rotation::axisYRotateVertex3(tmp, angle)) : std::move(Rotation::axisYRotateVertex3(tmp, -angle)));
         tmp += camera.getEye();

         camera.setLookAt(tmp);

         prevYPos = yPos;
      }
   }
}

void scrollCallBack(GLFWwindow *window, double xOffset, double yOffset) {
   if (DEVELOPER_MODE) {
      if (yOffset > 0) {
         camera.setNear(camera.getNear() + 0.075f);
      } else if (yOffset < 0) {
         camera.setNear(camera.getNear() - 0.075f);
      }
   }
}