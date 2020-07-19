#ifndef COTECCHIOGAME_GL_RENDERVARIABLES_H
#define COTECCHIOGAME_GL_RENDERVARIABLES_H

#include "GL_Loader.h"

// TODO setup as private struct (deny any part of the code to edit some of these variables in any way)
// Use namespaces?
// Collezione indici per inviare dati allo shader
extern GLuint projectionMatrixUniform;
extern GLuint viewMatrixUniform;
extern GLuint modelMatrixUniform;

extern GLuint texUnif;
extern GLuint bumpUnif;

extern GLuint tangentUniform;
extern GLuint bitangentUniform;

extern GLuint lightPosUniform;
extern GLuint lightColorUniform;
extern GLuint lightIntensity;

extern GLuint ambientCoefficient;
extern GLuint diffusiveCoefficient;
extern GLuint specularCoefficient;
extern GLuint specularAlpha;
extern GLuint eyePosition;

extern GLuint gammaUniform;

extern GLuint fixedColorUniform;

extern GLuint blurUniform;

extern GLuint cardModelMatrix;
extern GLuint cardViewMatrix;
extern GLuint cardProjectionMatrix;
extern GLuint playerIndexUniform;
extern GLuint viewPlayerUniform;

extern GLuint cardTexUnif;
extern GLuint backTexUnif;

extern GLuint deckModelMatrix;
extern GLuint deckViewMatrix;
extern GLuint deckProjectionMatrix;

extern GLuint deckCardTexUnif;

extern GLuint colorModelMatrix;
extern GLuint colorViewMatrix;
extern GLuint colorProjectionMatrix;

extern GLuint lightSpaceMatrixUniform;
extern GLuint modelLightShaderUniform;
extern GLuint lsmMainShaderUniform;
extern GLuint depthMapUniform;

extern GLuint nearPlaneUniform;
extern GLuint farPlaneUniform;

extern GLuint offlineFBTextureUniform;

extern SquareMatrix projM_V2C;
extern SquareMatrix viewM_W2V;
extern SquareMatrix viewM_V2W;
//SquareMatrix modelM_L2W(4, {});

extern SquareMatrix cardModelM;

extern std::vector<SquareMatrix> lightSpaceMs;

extern unsigned int skipVertexIndex;
extern unsigned int skipTextureIndex;
extern unsigned int skipBumpIndex;

extern unsigned int flipCardRenderingIndex;


#endif //COTECCHIOGAME_GL_RENDERVARIABLES_H
