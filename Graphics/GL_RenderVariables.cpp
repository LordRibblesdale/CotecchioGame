#include "GL_RenderVariables.h"

GLuint projectionMatrixUniform;
GLuint viewMatrixUniform;
GLuint modelMatrixUniform;

GLuint textureUniform;
GLuint bumpUniform;

GLuint tangentUniform;
GLuint bitangentUniform;

GLuint lightPosUniform;
GLuint lightColorUniform;
GLuint lightIntensity;

GLuint ambientCUniform;
GLuint diffusiveCUniform;
GLuint specularCUniform;
GLuint specularAlphaUniform;
GLuint eyePositionUniform;

GLuint gammaUniform;
GLuint blurUniform;

GLuint cardModelMatrix;
GLuint cardViewMatrix;
GLuint cardProjectionMatrix;
GLuint playerIndexUniform;
GLuint viewPlayerUniform;

GLuint cardTexUniform;
GLuint backTexUniform;
GLuint cardValueUniform;
GLuint deckCardValueUniform;

GLuint deckModelMatrix;
GLuint deckViewMatrix;
GLuint deckProjectionMatrix;

GLuint deckCardTexUniform;

GLuint colorModelMatrix;
GLuint colorViewMatrix;
GLuint colorProjectionMatrix;

GLuint lightSpaceMatrixUniform;
GLuint modelLightShaderUniform;
GLuint lsmMainShaderUniform;
GLuint depthMapUniform;

GLuint offlineFBTextureUniform;

GLuint nearPlaneUniform;
GLuint farPlaneUniform;
GLuint debugMapUniform;

SquareMatrix projM_V2C(4, {});
SquareMatrix viewM_W2V(4, {});
SquareMatrix viewM_V2W(4, {});

SquareMatrix cardModelM(4, {});

SquareMatrix lightSpaceMs(4, {});

unsigned int skipVertexIndex = 0;
unsigned int skipTextureIndex = 0;
unsigned int skipBumpIndex = 0;

unsigned int flipCardRenderingIndex;
