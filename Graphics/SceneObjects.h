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

#include "../Utilities/Camera/PerspectiveCamera.h"
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

extern GLuint shadowMapFrameBuffer;
extern GLuint shadowMapTexture;

//--------------------------------------------------------------//

extern GLuint sceneToOfflineRenderingShader;
extern GLuint offlineToWindowShader;
extern GLuint cardsShader;
extern GLuint deckShader;
extern GLuint outlinerShader;
extern GLuint shadowMapShader;
extern GLuint debugShader;

extern GLuint offlineFrameBuffer;
extern GLuint offlineTexture;
extern GLuint offlineRenderBufferObject;
extern GLuint msaaSupportFrameBuffer;
extern GLuint msaaOfflineTexture;

extern std::vector<GLuint> vertexArrayObjects;   // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
extern std::vector<GLuint> vertexBufferObjects;  // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
extern std::vector<GLuint> elementBufferObjects; // Element Buffer Object, buffer per la lettura dei vertici nel senso di topologia/mesh

extern std::vector<GLuint> textureUniforms;
extern std::vector<GLuint> bumpUniforms;

extern GLuint cardTexture;
extern GLuint backCardTexture;

extern GLuint cardVAO;
extern GLuint cardVBO;
extern GLuint cardEBO;

extern float screen[20];
extern float debugScreen[20];
extern unsigned int screenIndices[6];
extern GLuint screenVAO;
extern GLuint screenVBO;
extern GLuint screenEBO;

extern GLuint debugScreenVAO, debugScreenVBO, debugScreenEBO;

//--------------------------------------------------------------//

extern float aspectRatio;

extern Float3 defaultLookAt;
extern PerspectiveCamera camera;
extern float maxZ;

extern double prevXPos, prevYPos;
extern double prevTime, currTime, sumTimeTranslCamera;

extern std::unique_ptr<CameraRotation> cameraRotation;

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

extern std::unique_ptr<SpotLight> light;

//--------------------------------------------------------------//

// Questa variabile è a solo scopo dimostrativo, nel progetto effettivo sarà uno solo il giocatore
extern std::vector<Player> players;

//--------------------------------------------------------------//

extern bool DEVELOPER_MODE;
extern bool IS_ANIMATION_BUSY;
extern bool IS_GAME_STARTED;
extern double backspaceButtonTime;
extern double sumBSBTime;

#endif //COTECCHIOGAME_SCENEOBJECTS_H
