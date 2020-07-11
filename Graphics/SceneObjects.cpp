#include "SceneObjects.h"

GLFWwindow* window = nullptr;

GLuint phongShaderProgram;
GLuint offlineShaderProgram;
GLuint cardsShader;
GLuint backShader;
GLuint colorShader;
GLuint lightShader;

GLuint offlineFrameBuffer;
GLuint offlineTexture;
GLuint offlineRenderBufferObject;
GLuint secondaryFrameBuffer;
GLuint msaaOfflineTexture;

std::vector<GLuint> vertexArrayObjects;
std::vector<GLuint> vertexBufferObjects;
std::vector<GLuint> elementBufferObjects;

std::vector<GLuint> textureUniforms;
std::vector<GLuint> bumpUniforms;

GLuint cardTexture;
GLuint backCardTexture;

GLuint cardVAO;
GLuint cardVBO;
GLuint cardVBO2;
GLuint cardVBO3;
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

float aspectRatio = 540.0f/960.0f;

Float3 lookAt(0, 0, 5);
Camera camera(std::move(Float3(0, -20, 10)), lookAt, std::move(Float3(0, 0, 1)),
              0.2f, 0.05f, 1000, 60, aspectRatio);
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

float blurValue;

unsigned int playerIndex = 0;
unsigned int previousPlayerIndex = 0;
unsigned short int sessionPlayers = 0;

std::vector<Model> objects;
std::stack<unsigned int> cardsValue;

std::vector<Material> materials;
std::vector<unsigned int> materialIndices;

const std::string TABLE_ASSETS_LOCATION("DATA_ASSETS\\Table_OBJ\\");
const std::string DATA_ASSETS_LOCATION("DATA_ASSETS\\");

// TODO setup light decay
std::vector<std::unique_ptr<Light>> lights;

std::vector<Player> players;

bool DEVELOPER_MODE = false;
bool IS_ANIMATION_BUSY = false;
bool IS_GAME_STARTED = false;