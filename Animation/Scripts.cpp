#include "Scripts.h"

#include <memory>

void increaseIndex() {
   previousPlayerIndex = playerIndex;

   if (++playerIndex == sessionPlayers) {
      playerIndex = 0;
   }
}

void createPlayerPositions(unsigned short int nPlayers) {
   sessionPlayers = nPlayers;

   float angle = (M_PI * 2.0f) / static_cast<float>(nPlayers);

   // TODO use dynamic index
   maxZ = objects.at(1).getHighestZPoint();
   // TODO fix FurthestPoint
   //maxX = objects.at(0).getFurthestXPoint();

   players.reserve(nPlayers);

   //TODO use maxX for effective radius
   for (int i = 0; i < nPlayers; ++i) {
      players.emplace_back(Player(40/nPlayers, Float3(8.25f* cosf(i * angle), 9.25f*sinf(i * angle), maxZ+2)));
   }
}

void pollInput(GLFWwindow *window) {
   //TODO add time-based input
   float speed = 0.01f + ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 0.04f : 0);

   // Funzione per l'input, esempio via tastiera
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
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

   if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
      cameraPlayerPositionTime = 1;

      PLAYER_TRANSLATION_CAMERA = true;
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

   if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
      cameraPlayerPositionTime = 2;
      MENU_TRANSLATION_CAMERA = true;
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

   //---------------------------------------------------------------------------------------------------//

   if (TRANSFORM_CAMERA) {
      /*
      double diff = currTime - prevTime;
      if (!cameraRotation) {
         //cameraRotation = std::make_unique<CameraRotation>(camera.getEye(), position1, 0);
      }

      if (sumTime + diff <= cameraAnimationTime) {
         camera.setEye(cameraRotation->rotateCamera(camera.getEye(), diff));

         sumTime += diff;
      } else {
         //camera.setEye(position1);
         sumTime = 0;
         TRANSFORM_CAMERA = false;
      }
       */
   }
}

void cursorPositionCallBack(GLFWwindow *window, double xPos, double yPos) {
   if (DEVELOPER_MODE) {
      float diffX = camera.getSensibility() * (xPos - prevXPos);
      float diffY = camera.getSensibility() * (yPos - prevYPos);

      if (diffX != 0) {
         float angle = atanf(diffX/100.0f);

         Float3 tmp(move((camera.getLookAt() - camera.getEye()).getNormalized()));
         // TODO implement methods with reference access (reduce copies)
         tmp = move(Rotation::axisZRotateVertex3(tmp, -angle));

         camera.setYawAngle(acosf(tmp.getNormalized().getX()));

         tmp += camera.getEye();
         camera.setLookAt(tmp);

         prevXPos = xPos;
      }

      if (diffY != 0) {
         float angle = atanf(diffY/100.0f);

         Float3 tmp(move((camera.getLookAt() - camera.getEye()).getNormalized()));

         //TODO fix angle rotation
         //tmp = move(Rotation::axisZRotateVertex3(tmp, camera.getYawAngle()*0.5f));
         tmp = (tmp.getY() < 0 ? std::move(Rotation::axisXRotateVertex3(tmp, angle)) : std::move(Rotation::axisXRotateVertex3(tmp, -angle)))
               + (tmp.getX() > 0 ? std::move(Rotation::axisYRotateVertex3(tmp, angle)) : std::move(Rotation::axisYRotateVertex3(tmp, -angle)));// + std::move(Rotation::axisYRotateVertex3(tmp, -angle));
         //tmp = move(Rotation::axisZRotateVertex3(tmp, -camera.getYawAngle()*0.5f));
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
