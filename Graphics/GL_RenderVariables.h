#ifndef COTECCHIOGAME_GL_RENDERVARIABLES_H
#define COTECCHIOGAME_GL_RENDERVARIABLES_H

#include "GL_Loader.h"

// Collezione indici per inviare dati allo shader
extern GLuint projectionMatrixUniform;
extern GLuint viewMatrixUniform;
extern GLuint modelMatrixUniform;

extern GLuint textureUniform;
extern GLuint bumpUniform;

extern GLuint tangentUniform;
extern GLuint bitangentUniform;

extern GLuint lightPosUniform;
extern GLuint lightColorUniform;
extern GLuint lightIntensity;

extern GLuint ambientCUniform;
extern GLuint diffusiveCUniform;
extern GLuint specularCUniform;
extern GLuint specularAlphaUniform;
extern GLuint eyePositionUniform;
extern GLuint lookAtUniform;

extern GLuint gammaUniform;

extern GLuint fixedColorUniform;

extern GLuint blurUniform;

extern GLuint cardModelMatrix;
extern GLuint cardViewMatrix;
extern GLuint cardProjectionMatrix;
extern GLuint playerIndexUniform;
extern GLuint viewPlayerUniform;

extern GLuint cardTexUniform;
extern GLuint backTexUniform;
extern GLuint cardValueUniform;
extern GLuint deckCardValueUniform;

extern GLuint deckModelMatrix;
extern GLuint deckViewMatrix;
extern GLuint deckProjectionMatrix;

extern GLuint deckCardTexUniform;

extern GLuint colorModelMatrix;
extern GLuint colorViewMatrix;
extern GLuint colorProjectionMatrix;

extern GLuint lightSpaceMatrixUniform;
extern GLuint modelLightShaderUniform;
extern GLuint lsmMainShaderUniform;
extern GLuint depthMapUniform;

extern GLuint offlineFBTextureUniform;

extern GLuint nearPlaneUniform;
extern GLuint farPlaneUniform;
extern GLuint debugMapUniform;

extern SquareMatrix projM_V2C;
extern SquareMatrix viewM_W2V;
extern SquareMatrix viewM_V2W;

extern SquareMatrix cardModelM;

extern SquareMatrix lightSpaceMs;

extern unsigned int skipVertexIndex;
extern unsigned int skipTextureIndex;
extern unsigned int skipBumpIndex;

extern unsigned int flipCardRenderingIndex;


#endif //COTECCHIOGAME_GL_RENDERVARIABLES_H
