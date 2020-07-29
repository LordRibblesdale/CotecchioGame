#include "GL_Loader.h"
#include "GL_RenderVariables.h"

void renderText() {

}

void setupRenderVariables() {
   // Collezione indici per inviare dati allo shader
   projectionMatrixUniform = glGetUniformLocation(shaderProgram, "projection");
   viewMatrixUniform = glGetUniformLocation(shaderProgram, "view");
   modelMatrixUniform = glGetUniformLocation(shaderProgram, "model");

   textureUniform = glGetUniformLocation(shaderProgram, "texture1");
   bumpUniform = glGetUniformLocation(shaderProgram, "normalTexture");

   tangentUniform = glGetUniformLocation(shaderProgram, "tangent");
   bitangentUniform = glGetUniformLocation(shaderProgram, "bitangent");

   lightPosUniform = glGetUniformLocation(shaderProgram, "lightPos");
   lightColorUniform = glGetUniformLocation(shaderProgram, "lightColor");
   lightIntensity = glGetUniformLocation(shaderProgram, "lightIntensity");

   ambientCUniform = glGetUniformLocation(shaderProgram, "ambientCoefficient");
   diffusiveCUniform = glGetUniformLocation(shaderProgram, "diffusiveCoefficient");
   specularCUniform = glGetUniformLocation(shaderProgram, "specularCoefficient");
   specularAlphaUniform = glGetUniformLocation(shaderProgram, "specularAlpha");
   eyePositionUniform = glGetUniformLocation(shaderProgram, "eye");
   lookAtUniform = glGetUniformLocation(shaderProgram, "lookAt");

   gammaUniform = glGetUniformLocation(shaderProgram, "gammaCorrection");

   fixedColorUniform = glGetUniformLocation(shaderProgram, "color");

   blurUniform = glGetUniformLocation(offlineShaderProgram, "blurValue");

   cardModelMatrix = glGetUniformLocation(cardsShader, "model");
   cardViewMatrix = glGetUniformLocation(cardsShader, "view");
   cardProjectionMatrix = glGetUniformLocation(cardsShader, "projection");
   playerIndexUniform = glGetUniformLocation(cardsShader, "currentPlayer");
   viewPlayerUniform = glGetUniformLocation(cardsShader, "viewPlayer");

   cardTexUniform = glGetUniformLocation(cardsShader, "cardTexture");
   backTexUniform = glGetUniformLocation(cardsShader, "backTexture");
   cardValueUniform = glGetUniformLocation(cardsShader, "cardValue");
   deckCardValueUniform = glGetUniformLocation(deckShader, "cardValue");

   deckModelMatrix = glGetUniformLocation(deckShader, "model");
   deckViewMatrix = glGetUniformLocation(deckShader, "view");
   deckProjectionMatrix = glGetUniformLocation(deckShader, "projection");

   deckCardTexUniform = glGetUniformLocation(deckShader, "cardTexture");

   colorModelMatrix = glGetUniformLocation(colorShader, "model");
   colorViewMatrix = glGetUniformLocation(colorShader, "view");
   colorProjectionMatrix = glGetUniformLocation(colorShader, "projection");

   lightSpaceMatrixUniform = glGetUniformLocation(lightShader, "lightSpaceMatrix");
   modelLightShaderUniform = glGetUniformLocation(lightShader, "modelMatrix");
   lsmMainShaderUniform = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
   depthMapUniform = glGetUniformLocation(shaderProgram, "depthMap");

   offlineFBTextureUniform = glGetUniformLocation(offlineShaderProgram, "offlineRendering");

   nearPlaneUniform = glGetUniformLocation(debugShader, "near_plane");
   farPlaneUniform = glGetUniformLocation(debugShader, "far_plane");
   debugMapUniform = glGetUniformLocation(debugShader, "depthMap");
}

void renderShadowMap() {
   // Rendering shadow map
   glUseProgram(lightShader);

   // Imposto la viewport per il render della shadow map
   glViewport(0, 0, SHADOW_QUALITY, SHADOW_QUALITY);

   // Definisce la normale da calcolare in base all'ordine dei vertici (se come orari o antiorari)
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_FRONT);
   //glFrontFace(GL_CW); // o CCW

   skipVertexIndex = 0;

   glBindFramebuffer(GL_FRAMEBUFFER, lightFrameBuffer);
   glClear(GL_DEPTH_BUFFER_BIT);

   lightSpaceMs = std::move(Projection::onAxisFOV2ClipProjectiveMatrix(*light->getCamera()) * light->getCamera()->world2ViewMatrix());

   glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_TRUE, lightSpaceMs.getArray());

   for (auto & object : objects) {
      glUniformMatrix4fv(modelLightShaderUniform, 1, GL_TRUE, object.getWorldCoordinates().getArray());

      for (auto j = 0; j < object.getMeshes().size(); ++j) {
         if (object.doesNeedNoCulling()) {
            glDisable(GL_CULL_FACE);
         }

         glBindVertexArray(vertexArrayObjects.at(skipVertexIndex++));
         // Chamata di disegno della primitiva
         glDrawElements(GL_TRIANGLES, object.getMeshes().at(j).getIndices().size(), GL_UNSIGNED_INT, 0);

         if (object.doesNeedNoCulling()) {
            glEnable(GL_CULL_FACE);
         }
      }
   }

   glDisable(GL_CULL_FACE);

   if (!players.empty()) {
      glBindVertexArray(cardVAO);

      for (Player& player : players) {
         for (unsigned int i = 0; i < player.getCards().size(); ++i) {
            cardModelM = std::move(player.getCards().at(i).getWorldCoordinates(i));
            glUniformMatrix4fv(modelLightShaderUniform, 1, GL_TRUE, cardModelM.getArray());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         }
      }
   }

   glDisable(GL_DEPTH_TEST);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderSceneObjects() {
   glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);
   glViewport(0, 0, X_RESOLUTION, Y_RESOLUTION);

   glClearColor(0.2, 0.2, 0.2, 1.0f);
   // Pulizia buffer colore, depth e stencil
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);   // Clear dello stato del sistema

   // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
   glUseProgram(shaderProgram);

   // Definisce la normale da calcolare in base all'ordine dei vertici (se come orari o antiorari)
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   //glFrontFace(GL_CW); // o CCW

   // Abilito il depth test per il check della profondità per la stampa a video degli oggetti
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   // Per ora viene disattivata la scrittura sul buffer
   glStencilMask(0x00);

   /* Scrivere nella location della variabile i valori del colore da assegnare al pixel;
    * Essendo macchina di stato, bisogna ricordare che la posizione influisce sull'azione delle chiamate
    * Quindi attenzione al posizionamento delle chiamate di modifica stato
    */

   projM_V2C = std::move(Projection::onAxisFOV2ClipProjectiveMatrix(camera));
   viewM_V2W = std::move(camera.view2WorldMatrix());
   viewM_W2V = std::move(SquareMatrix::calculateInverse(viewM_V2W));

   glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, projM_V2C.getArray());
   glUniformMatrix4fv(viewMatrixUniform, 1, GL_TRUE, viewM_W2V.getArray());
   glUniformMatrix4fv(lsmMainShaderUniform, 1, GL_TRUE, lightSpaceMs.getArray());

   glUniform3f(eyePositionUniform, camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ());
   glUniform3f(lookAtUniform, camera.getLookAt().getX(), camera.getLookAt().getY(), camera.getLookAt().getZ());

   glUniform3f(lightPosUniform, light->getOrigin().getX(), light->getOrigin().getY(), light->getOrigin().getZ());
   glUniform3f(lightColorUniform, light->getColor().getRed(), light->getColor().getGreen(), light->getColor().getBlue());
   glUniform1f(lightIntensity, light->getIntensity());

   // Caricare vertexArrayObject interessato
   glUniform1i(textureUniform, 0);
   glUniform1i(bumpUniform, 1);
   glUniform1i(depthMapUniform, 10);

   glActiveTexture(GL_TEXTURE10);
   //glBindTexture(GL_TEXTURE_2D, lights.at(0)->getDepthMapAsReference());
   glBindTexture(GL_TEXTURE_2D, lightTexture);

   glUniform1f(gammaUniform, GAMMA_CORRECTION);

   skipVertexIndex = 0;
   skipTextureIndex = 0;
   skipBumpIndex = 0;

   for (auto & object : objects) {
      //modelM_L2W = object.getWorldCoordinates();

      //glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, modelM_L2W.getArray());
      //glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, object.getLocal2WorldMatrix().getArray());
      glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, object.getWorldCoordinates().getArray());

      for (auto j = 0; j < object.getMeshes().size(); ++j) {
         if (object.doesHaveTextures()) {
            glUniform3f(ambientCUniform, materials.at(materialIndices.at(j)).ambientCoeff.getX(),
                        materials.at(materialIndices.at(j)).ambientCoeff.getY(),
                        materials.at(materialIndices.at(j)).ambientCoeff.getZ());
            glUniform3f(diffusiveCUniform, materials.at(materialIndices.at(j)).diffuseCoeff.getX(),
                        materials.at(materialIndices.at(j)).diffuseCoeff.getY(),
                        materials.at(materialIndices.at(j)).diffuseCoeff.getZ());
            glUniform3f(specularCUniform, materials.at(materialIndices.at(j)).specularCoeff.getX(),
                        materials.at(materialIndices.at(j)).specularCoeff.getY(),
                        materials.at(materialIndices.at(j)).specularCoeff.getZ());
            glUniform1f(specularAlphaUniform, materials.at(materialIndices.at(j)).shininess);

            // if (materialIndices.at(j) == 1) {}

            // Imposto lo uniform interessato con la texture unit in cui è presente la texture
            // Attivazione canale texture (Texture Unit), per poter utilizzare il canale (che dentro è presente una texture)
            glActiveTexture(GL_TEXTURE0);
            // Il bind sulla variabile texture1 ora si riverisce alla texture unit a cui è stata collegata
            glBindTexture(GL_TEXTURE_2D, textureUniforms.at(skipTextureIndex++));

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, bumpUniforms.at(skipBumpIndex++));

            glUniform3f(tangentUniform, object.getMeshes().at(j).getTangent().getX(),
                        object.getMeshes().at(j).getTangent().getY(),
                        object.getMeshes().at(j).getTangent().getZ());

            glUniform3f(bitangentUniform, object.getMeshes().at(j).getBitangent().getX(),
                        object.getMeshes().at(j).getBitangent().getY(),
                        object.getMeshes().at(j).getBitangent().getZ());
         } else {
            glUniform3f(ambientCUniform, materials.at(materials.size() - 1).ambientCoeff.getX(),
                        materials.at(materials.size()-1).ambientCoeff.getY(),
                        materials.at(materials.size()-1).ambientCoeff.getZ());

            //glUniform3f(fixedColorUniform, 0.2f, 0.2f, 0.2f);
         }

         if (object.doesNeedNoCulling()) {
            glDisable(GL_CULL_FACE);
         }

         glBindVertexArray(vertexArrayObjects.at(skipVertexIndex++));
         // Chamata di disegno della primitiva
         glDrawElements(GL_TRIANGLES, object.getMeshes().at(j).getIndices().size(), GL_UNSIGNED_INT, 0);

         if (object.doesNeedNoCulling()) {
            glEnable(GL_CULL_FACE);
         }
      }
   }
}

void renderCardsInLoop(unsigned int& pIndex, size_t& i, bool& hasSelectedCard, double x, double y) {
   //std::vector<SquareMatrix> matrices;
   //matrices.reserve(4);

   //players.at(pIndex).getCards().at(i).getWorldCoordinates(i, matrices);

   cardModelM = std::move(players.at(pIndex).getCards().at(i).getWorldCoordinates(i));

   if (pIndex == playerIndex) {
      if (!hasSelectedCard) {
         if ((x >= 0 && x <= X_RESOLUTION) && (y >= 0 && y <= Y_RESOLUTION)) {
            /* Increase performance from ([3*n^3 + 4*n^2] to [12*n^2])
               n = 4 -> 256 to 192 operations

            Float4 p0(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                            Float4(cardVertices[0], cardVertices[1], cardVertices[2], 1))))));
            Float4 p1(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                            Float4(cardVertices[6], cardVertices[7], cardVertices[8], 1))))));
            Float4 p2(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                    Float4(cardVertices[12], cardVertices[13], cardVertices[14], 1))))));
            Float4 p3(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                    Float4(cardVertices[18], cardVertices[19], cardVertices[20], 1))))));
                                    */

            Float4 p0(std::move(cardModelM.multiplyVector(Float4(cardVertices[0], cardVertices[1], cardVertices[2], 1))));
            Float4 p1(std::move(cardModelM.multiplyVector(Float4(cardVertices[8], cardVertices[9], cardVertices[10], 1))));
            Float4 p2(std::move(cardModelM.multiplyVector(Float4(cardVertices[16], cardVertices[17], cardVertices[18], 1))));
            Float4 p3(std::move(cardModelM.multiplyVector(Float4(cardVertices[24], cardVertices[25], cardVertices[26], 1))));

            Triangle cardT1(p0, p1, p2);
            Triangle cardT2(p2, p1, p3);

            // ATTENZIONE: questa impostazione vale per le on-axis camera
            // NDC -> Clip Space
            float xProj = (((2.0f*x)/static_cast<float>(X_RESOLUTION)) -1);
            float yProj = -(((2.0f*y)/static_cast<float>(Y_RESOLUTION)) -1);  // MINUS per via dell'asse invertito Y SCREEN rispetto a Y CLIP
            float zProj = -camera.getNear();

            // NDC Space -> View Space (ON-AXIS CAMERA)
            Float4 clipPoint(xProj * camera.getRight(), yProj * camera.getTop(), zProj, 1);

            // View Space -> World Space
            clipPoint = std::move(viewM_V2W.multiplyVector(clipPoint));

            Ray ray(camera.getEye(), std::move((Float3(clipPoint.getFloat3()) - camera.getEye()).getNormalized()));

            TriangleIntersection i1(ray.getTriangleIntersection(cardT1));
            TriangleIntersection i2(ray.getTriangleIntersection(cardT2));

            if (i1.getHasIntersected() || i2.getHasIntersected()) {
               players.at(pIndex).getCards().at(i).setIsSelected(true);

               hasSelectedCard = true;
               selectedCardIndex = i;

               glStencilMask(0xFF);
            }
         }
      }
   }

   glUniform1i(cardValueUniform, players.at(pIndex).getCards().at(i).value);

   glUniformMatrix4fv(cardModelMatrix, 1, GL_TRUE, cardModelM.getArray());

   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderCards() {
   glUseProgram(cardsShader);

   glDisable(GL_CULL_FACE);

   // Blend di trasparenza
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

   glBindVertexArray(cardVAO);

   glUniform1i(cardTexUniform, 3);
   glUniform1i(backTexUniform, 4);

   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, cardTexture);

   glActiveTexture(GL_TEXTURE4);
   glBindTexture(GL_TEXTURE_2D, backCardTexture);

   glUniformMatrix4fv(cardProjectionMatrix, 1, GL_TRUE, projM_V2C.getArray());
   glUniformMatrix4fv(cardViewMatrix, 1, GL_TRUE, viewM_W2V.getArray());
   glUniform1i(playerIndexUniform, playerIndex);

   glStencilFunc(GL_ALWAYS, 1, 0xFF);
   glStencilMask(0x00);

   bool hasSelectedCard = false;
   double x, y;
   glfwGetCursorPos(window, &x, &y);

   if (!players.empty()) {
      for (unsigned int pIndex = 0; pIndex < players.size(); ++pIndex) {
         glUniform1i(viewPlayerUniform, pIndex);

         if (pIndex < flipCardRenderingIndex) {
            for (size_t i = 0; i < players.at(pIndex).getCards().size(); ++i) {
               renderCardsInLoop(pIndex, i, hasSelectedCard, x, y);
            }
         } else {
            for (size_t i = players.at(pIndex).getCards().size()-1; i >= 0 && i != MAX_SIZE_T_VALUE; --i) {
               renderCardsInLoop(pIndex, i, hasSelectedCard, x, y);
            }
         }

         if (hasSelectedCard && (playerIndex == pIndex)) {
            glDisable(GL_DEPTH_TEST);

            cardModelM = std::move(players.at(playerIndex).getCards().at(selectedCardIndex).getWorldCoordinates(selectedCardIndex));

            glUniform1i(cardValueUniform, players.at(pIndex).getCards().at(selectedCardIndex).value);
            glUniformMatrix4fv(cardModelMatrix, 1, GL_TRUE, cardModelM.getArray());

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glEnable(GL_DEPTH_TEST);
         }
      }

      // Genero outlining tramite Stencil test
      glUseProgram(colorShader);

      // Disabilito il Depth Test per poter aggiungere varie informazioni o effetti a schermo
      glDisable(GL_DEPTH_TEST);

      // Verrà stampato qualcosa su schermo solo se supera il controllo sottostante
      // Ovvero quando l'oggetto si trova fuori dalla maschera, ovvero il tratto per l'outlining
      glStencilMask(0x00);    // solo in lettura del buffer, non attiva la scrittura
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glEnable(GL_DEPTH_TEST);

      glUniformMatrix4fv(colorProjectionMatrix, 1, GL_TRUE, projM_V2C.getArray());
      glUniformMatrix4fv(colorViewMatrix, 1, GL_TRUE, viewM_W2V.getArray());

      if (selectedCardIndex != MAX_SIZE_T_VALUE) {
         cardModelM = std::move(players.at(playerIndex).getCards().at(selectedCardIndex).getLocal2World() * Transform::scaleMatrix4(1.05, 1.05, 1.05));
         glUniform1i(cardValueUniform, players.at(playerIndex).getCards().at(selectedCardIndex).value);
         glUniformMatrix4fv(colorModelMatrix, 1, GL_TRUE, cardModelM.getArray());
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         players.at(playerIndex).getCards().at(selectedCardIndex).setIsSelected(false);
      }
   }
}

void renderCardsOnTable() {
   glUseProgram(deckShader);
   glUniform1i(deckCardTexUniform, 3);

   // Needed?
   glBindVertexArray(cardVAO);
   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, cardTexture);

   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

   glUniformMatrix4fv(deckProjectionMatrix, 1, GL_TRUE, projM_V2C.getArray());
   glUniformMatrix4fv(deckViewMatrix, 1, GL_TRUE, viewM_W2V.getArray());

   for (const Card& card : cardsOnTable) {
      cardModelM = *card.hand2Table * card.local2World * *card.rotationOnTable;

      glUniform1i(deckCardValueUniform, card.value);
      glUniformMatrix4fv(deckModelMatrix, 1, GL_TRUE, cardModelM.getArray());

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }

   // Da riattivare per permettere le modifiche al buffer (pulizia)
   glStencilMask(0xFF);

   glDisable(GL_BLEND);
}

int printOglError(const char * file, int line) {
   GLenum glErr;
   int    retCode = 0;

   // gluErrorString was deprecated
   const auto gluErrorString = [](GLenum errorCode)->const char *
   {
      switch (errorCode)
      {
         default:
            return "unknown error code";
         case GL_NO_ERROR:
            return "no error";
         case GL_INVALID_ENUM:
            return "invalid enumerant";
         case GL_INVALID_VALUE:
            return "invalid value";
         case GL_INVALID_OPERATION:
            return "invalid operation";
#ifndef GL_VERSION_3_0
            case GL_STACK_OVERFLOW:
			return "stack overflow";
		case GL_STACK_UNDERFLOW:
			return "stack underflow";
		case GL_TABLE_TOO_LARGE:
			return "table too large";
#endif
         case GL_OUT_OF_MEMORY:
            return "out of memory";
#ifdef GL_EXT_framebuffer_object
            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
			return "invalid framebuffer operation";
#endif
      }
   };

   glErr = glGetError();
   if (glErr != GL_NO_ERROR)
   {
      printf("glError in file %s @ line %d: %s\n",
             file, line, gluErrorString(glErr));
      retCode = 1;
   }
   return retCode;
}
