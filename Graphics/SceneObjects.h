#ifndef COTECCHIOGAME_SCENEOBJECTS_H
#define COTECCHIOGAME_SCENEOBJECTS_H

#include <vector>
#include <stack>

#ifdef _WIN32
#include "glfw3.h"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif
#include <assimp/Importer.hpp>

#include "../Utilities/Camera.h"
#include "../Scripting/CameraRotation.h"
#include "../Scripting/CameraTranslation.h"
#include "../Model/Model.h"

#include "../Model/Light/SpotLight.h"
#include "../Matrix/StandardMatrix.h"

#include "../Model/Cards/Card.h"
#include "../Player/Player.h"
#include "../Scripting/CardMoving.h"

extern GLFWwindow* window;

extern std::unique_ptr<Assimp::Importer> importer;

//--------------------------------------------------------------//

extern GLuint shaderProgram;
extern GLuint offlineShaderProgram;
extern GLuint cardsShader;
extern GLuint deckShader;
extern GLuint colorShader;
extern GLuint lightShader;

extern GLuint offlineFrameBuffer;
extern GLuint offlineTexture;
extern GLuint offlineRenderBufferObject;
extern GLuint secondaryFrameBuffer;
extern GLuint msaaOfflineTexture;

extern std::vector<GLuint> vertexArrayObjects;   // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
extern std::vector<GLuint> vertexBufferObjects;  // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
extern std::vector<GLuint> elementBufferObjects; // Element Buffer Object, buffer per la lettura dei vertici nel senso di topologia/mesh

extern std::vector<GLuint> textureUniforms;
extern std::vector<GLuint> bumpUniforms;

extern GLuint cardTexture;
extern GLuint backCardTexture;

extern GLuint cardVAO;
extern GLuint cardBuffer;
extern GLuint editableUVCardBuffer;
extern GLuint backUVCardBuffer;
extern GLuint cardEBO;

extern float screen[20];
extern unsigned int screenIndices[6];
extern GLuint sVAO;
extern GLuint sVBO;
extern GLuint sEBO;

//--------------------------------------------------------------//

extern float aspectRatio;

extern Float3 lookAt;
extern Camera camera;
extern float maxX;
extern float maxY;
extern float maxZ;

extern double prevXPos, prevYPos;
extern double prevTime, currTime, sumTimeTranslCamera;

extern bool TRANSFORM_CAMERA;
extern std::unique_ptr<CameraRotation> cameraRotation;
extern double cameraAnimationTime;

extern bool MENU_TRANSLATION_CAMERA;
extern bool PLAYER_TRANSLATION_CAMERA;
extern std::unique_ptr<CameraTranslation> cameraTranslation;
extern std::unique_ptr<CameraTranslation> cameraTranslation2;
extern double cameraPlayerPositionTime;

extern float blurValue;

extern unsigned int playerIndex;
extern unsigned short int sessionPlayers;

//--------------------------------------------------------------//

extern std::vector<Model> objects;
extern std::stack<unsigned int> cardsValue;
extern std::vector<Card> cardsOnTable;
extern std::unique_ptr<CardMoving> cardMovingAnimation;
extern size_t selectedCardIndex;
extern size_t MAX_SIZE_T_VALUE;
extern double cardAnimationTime;
extern double sumTimeCardAnimationTime;
extern bool CARD_ANIMATION;
extern bool BUSY_AT_CARD_ANIMATION;

extern std::vector<Material> materials;
extern std::vector<unsigned int> materialIndices;

extern const std::string TABLE_ASSETS_LOCATION;
extern const std::string DATA_ASSETS_LOCATION;

extern std::vector<std::unique_ptr<Light>> lights;

//--------------------------------------------------------------//

// Questa variabile è a solo scopo dimostrativo, nel progetto effettivo sarà uno solo il giocatore
extern std::vector<Player> players;

//--------------------------------------------------------------//

extern bool DEVELOPER_MODE;
extern bool IS_ANIMATION_BUSY;
extern bool IS_GAME_STARTED;

#endif //COTECCHIOGAME_SCENEOBJECTS_H
