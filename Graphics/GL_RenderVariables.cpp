#include "GL_RenderVariables.h"

GLuint projectionMatrixUniform;
GLuint viewMatrixUniform;
GLuint modelMatrixUniform;

GLuint texUnif;
GLuint bumpUnif;

GLuint tangentUniform;
GLuint bitangentUniform;

GLuint lightPosUniform;
GLuint lightColorUniform;
GLuint lightIntensity;

GLuint ambientCoefficient;
GLuint diffusiveCoefficient;
GLuint specularCoefficient;
GLuint specularAlpha;
GLuint eyePosition;

GLuint gammaUniform;
GLuint fixedColorUniform;
GLuint blurUniform;

GLuint cardModelMatrix;
GLuint cardViewMatrix;
GLuint cardProjectionMatrix;
GLuint playerIndexUniform;
GLuint viewPlayerUniform;

GLuint cardTexUnif;
GLuint backTexUnif;

GLuint deckModelMatrix;
GLuint deckViewMatrix;
GLuint deckProjectionMatrix;

GLuint deckCardTexUnif;

GLuint colorModelMatrix;
GLuint colorViewMatrix;
GLuint colorProjectionMatrix;

GLuint lightSpaceMatrixUniform;
GLuint modelLightShaderUniform;
GLuint lsmMainShaderUniform;
GLuint depthMapUniform;

GLuint offlineFBTextureUniform;

SquareMatrix projM_V2C(4, {});
SquareMatrix viewM_W2V(4, {});
SquareMatrix viewM_V2W(4, {});

SquareMatrix cardModelM(4, {});

std::vector<SquareMatrix> lightSpaceMs;

unsigned int skipVertexIndex = 0;
unsigned int skipTextureIndex = 0;
unsigned int skipBumpIndex = 0;

unsigned int flipCardRenderingIndex;
