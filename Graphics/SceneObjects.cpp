#include "SceneObjects.h"

GLFWwindow* window = nullptr;

const unsigned short int PHONG_SHADER = 0;
const unsigned short int COLOR_ONLY_SHADER = 1;

GLuint phongShaderProgram;
GLuint offlineShaderProgram;
GLuint cardsShader;

GLuint offlineFrameBuffer;
GLuint offlineTexture;
GLuint offlineRenderBufferObject;

std::vector<GLuint> vertexArrayObjects;
std::vector<GLuint> vertexBufferObjects;
std::vector<GLuint> elementBufferObjects;

std::vector<GLuint> textureUniforms;
std::vector<GLuint> bumpUniforms;

GLuint cardTexture;
GLuint backCardTexture;

GLuint cardVAO;
GLuint cardVBO;
GLuint cardEBO;

float screen[20] {
   -1, 1, 0, 0, 1,
   -1, -1, 0, 0, 0,
   1, -1, 0, 1, 0,
   1, 1, 0, 1, 1
};

unsigned int screenIndices[6] {
   0, 1, 2, 0, 2, 3
};

GLuint sVAO;
GLuint sVBO;
GLuint sEBO;

float aspectRatio;

Float3 lookAt(0, 0, 5);
Camera camera(std::move(Float3(0, 20, 15)), lookAt, std::move(Float3(0, 0, 1)),
              0.3f, 1000, 40, aspectRatio);
float maxX = 0;
float maxY = 0;
float maxZ = 0;

double prevXPos, prevYPos;
double prevTime, currTime, sumTime, sumTimeTranslCamera;

bool TRANSFORM_CAMERA = false;
std::unique_ptr<CameraRotation> cameraRotation;
double cameraAnimationTime;

bool MENU_TRANSLATION_CAMERA = false;
bool PLAYER_TRANSLATION_CAMERA = false;
std::unique_ptr<CameraTranslation> cameraTranslation;
std::unique_ptr<CameraTranslation> cameraTranslation2;
double cameraPlayerPositionTime;

GLuint blurUniform;
float blurValue = 1;

unsigned int playerIndex = 0;
unsigned short int sessionPlayers = 0;

std::vector<Model> objects;
std::stack<unsigned int> cardsValue;

std::vector<Material> materials;
std::vector<unsigned int> materialIndices;

const std::string TABLE_ASSETS_LOCATION("DATA_ASSETS\\Table_OBJ\\");
const std::string DATA_ASSETS_LOCATION("DATA_ASSETS\\");

// TODO setup light decay
SpotLight light(std::move(Float3(0, 0, 20)), Color(1, 1, 1), 10, degree2Radiants(40), degree2Radiants(60));

std::vector<Player> players;

bool DEVELOPER_MODE = false;
bool IS_ANIMATION_BUSY = false;
bool IS_GAME_STARTED = false;