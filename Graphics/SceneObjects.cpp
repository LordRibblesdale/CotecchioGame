#include "SceneObjects.h"

GLFWwindow* window = nullptr;
std::unique_ptr<Assimp::Importer> importer;

GLuint lightFrameBuffer;
GLuint lightTexture;

GLuint shaderProgram;
GLuint offlineShaderProgram;
GLuint cardsShader;
GLuint deckShader;
GLuint colorShader;
GLuint lightShader;
GLuint debugShader;

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
GLuint cardEBO;

float screen[20] {
   -1, 1, 0, 0, 1,
   -1, -1, 0, 0, 0,
   1, -1, 0, 1, 0,
   1, 1, 0, 1, 1
};

float screen2[20] {
   -0.9, -0.6, 0, 0, 1,
   -0.9, -0.9, 0, 0, 0,
   -0.6, -0.9, 0, 1, 0,
   -0.6, -0.6, 0, 1, 1
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
double prevTime, currTime, sumTimeTranslCamera;

bool TRANSFORM_CAMERA = false;
std::unique_ptr<CameraRotation> cameraRotation;
double cameraAnimationTime;

bool MENU_TRANSLATION_CAMERA = false;
bool PLAYER_TRANSLATION_CAMERA = false;
std::unique_ptr<CameraTranslation> cameraTranslation;
std::unique_ptr<CameraTranslation> cameraTranslation2;
double cameraPlayerPositionTime = 1;

float blurValue;

unsigned int playerIndex = 0;
unsigned short int sessionPlayers = 0;

std::vector<Model> objects;
std::stack<unsigned int> cardsValue;
std::vector<Card> cardsOnTable;
std::unique_ptr<CardMoving> cardMovingAnimation;
size_t selectedCardIndex;
size_t MAX_SIZE_T_VALUE;
double cardAnimationTime;
double sumTimeCardAnimationTime;
bool CARD_ANIMATION = false;
bool BUSY_AT_CARD_ANIMATION = false;

std::vector<Material> materials;
std::vector<unsigned int> materialIndices;

const std::string TABLE_ASSETS_LOCATION("DATA_ASSETS\\Table_OBJ\\");
const std::string DATA_ASSETS_LOCATION("DATA_ASSETS\\");

std::unique_ptr<Light> light;

std::vector<Player> players;

bool DEVELOPER_MODE = false;
bool IS_ANIMATION_BUSY = false;
bool IS_GAME_STARTED = false;