#ifndef COTECCHIOGAME_SCENEOBJECTS_H
#define COTECCHIOGAME_SCENEOBJECTS_H

#include <vector>
#include <glfw3.h>

#include "../Utilities/Camera.h"
#include "../Animation/CameraRotation.h"
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

extern Camera camera;
extern Float3 position1;

extern double cameraAnimationTime;

extern double prevXPos, prevYPos;
extern double prevTime, currTime, sumTime;

extern bool TRANSFORM_CAMERA;
extern std::unique_ptr<CameraRotation> cameraRotation;

//--------------------------------------------------------------//

extern std::vector<Model> objects;

extern const std::string TABLE_ASSETS_LOCATION;

extern SpotLight light;

#endif //COTECCHIOGAME_SCENEOBJECTS_H
