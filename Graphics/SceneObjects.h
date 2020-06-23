#ifndef COTECCHIOGAME_SCENEOBJECTS_H
#define COTECCHIOGAME_SCENEOBJECTS_H

#include <vector>
#include <glfw3.h>

#include "../Utilities/Camera.h"
#include "../Animation/CameraRotation.h"
#include "../Animation/CameraTranslation.h"
#include "../Model/Model.h"

#include "../Model/Light/SpotLight.h"
#include "../Matrix/StandardMatrix.h"

extern GLFWwindow* window;

//--------------------------------------------------------------//

extern const unsigned short int PHONG_SHADER;
extern const unsigned short int COLOR_ONLY_SHADER;

extern GLuint phongShaderProgram;
extern GLuint offlineShaderProgram;

extern GLuint offlineFrameBuffer;
extern GLuint offlineTexture;
extern GLuint offlineRenderBufferObject;

extern std::vector<GLuint> vertexArrayObjects;   // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
extern std::vector<GLuint> vertexBufferObjects;  // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
extern std::vector<GLuint> elementBufferObjects;

extern std::vector<GLuint> textureUniforms;
extern std::vector<GLuint> bumpUniforms;

extern float screen[20];
extern unsigned int screenIndices[6];
extern GLuint sVAO;
extern GLuint sVBO;
extern GLuint sEBO;

//--------------------------------------------------------------//

extern int WIDTH;
extern int HEIGHT;
extern float aspectRatio;

extern Float3 lookAt;
extern Camera camera;
extern float maxX;
extern float maxY;
extern float maxZ;

extern std::vector<Float3> playerPositions;

extern double prevXPos, prevYPos;
extern double prevTime, currTime, sumTimeCamera, sumTimeTranslCamera;

extern bool TRANSFORM_CAMERA;
extern std::unique_ptr<CameraRotation> cameraRotation;
extern double cameraAnimationTime;

extern bool MENU_TRANSLATION_CAMERA;
extern bool PLAYER_TRANSLATION_CAMERA;
extern std::unique_ptr<CameraTranslation> cameraTranslation;
extern std::unique_ptr<CameraTranslation> cameraTranslation2;
extern double cameraPlayerPositionTime;

extern GLuint blurUniform;
extern float blurValue;

extern unsigned int playerIndex;
extern unsigned short int sessionPlayers;

//--------------------------------------------------------------//

extern std::vector<Model> objects;

extern const std::string TABLE_ASSETS_LOCATION;

extern SpotLight light;

#endif //COTECCHIOGAME_SCENEOBJECTS_H
