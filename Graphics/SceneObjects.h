#ifndef COTECCHIOGAME_SCENEOBJECTS_H
#define COTECCHIOGAME_SCENEOBJECTS_H

#include "../Model/Light/SpotLight.h"
#include "../Animation/CameraRotation.h"

GLFWwindow* window = nullptr;

//--------------------------------------------------------------//

const unsigned short int PHONG_SHADER = 0;
const unsigned short int COLOR_ONLY_SHADER = 1;

GLuint phongShaderProgram;
GLuint offlineShaderProgram;

GLuint offlineFrameBuffer;

std::vector<GLuint> vertexArrayObjects;   // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
std::vector<GLuint> vertexBufferObjects;  // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
std::vector<GLuint> elementBufferObjects;

std::vector<GLuint> textureUniforms;
std::vector<GLuint> bumpUniforms;

//--------------------------------------------------------------//

int WIDTH = 960;
int HEIGHT = 540;
float aspectRatio = static_cast<float>(HEIGHT) / static_cast<float>(WIDTH);

Camera camera(std::move(Float3(0, 10, 15)), std::move(Float3(0, 0, 0)), std::move(Float3(0, 0, 1)),
              0.4f, 1000, 35, aspectRatio);
Float3 position1(0, 10, 0);
double cameraAnimationTime;

double prevXPos, prevYPos;
double prevTime, currTime, sumTime;

bool TRANSFORM_CAMERA = false;
std::unique_ptr<CameraRotation> cameraRotation;

//--------------------------------------------------------------//

std::vector<Model> objects;

const std::string TABLE_ASSETS_LOCATION("DATA_ASSETS\\Table_OBJ\\");

SpotLight light(std::move(Float3(0, 0, 20)), Color(1, 1, 1), 10, degree2Radiants(40), degree2Radiants(60));

#endif //COTECCHIOGAME_SCENEOBJECTS_H
