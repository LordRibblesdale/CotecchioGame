#ifndef GL_LOADER_H
#define GL_LOADER_H

#include "glad.h"

#ifdef _WIN32
#include "glfw3.h"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif

#include "../Model/Mesh.h"
#include "Shaders.h"
#include "SceneObjects.h"
#include "Settings.h"
#include "../Scripting/Scripts.h"
#include "../Model/Triangle.h"
#include "../Utilities/Ray.h"

/* Processo di rendering:
 * - Inizializzazione libreria
 * - Inizializzazione finestra
 * - Caricamento del vertex shader e fragment shader
 * - Linking vertex shader e fragment shader nel program
 * - Compilazione del program
 * - Creazione dei buffer per inviare dati alla GPU
 * - Inizializzazione vertex array object per contenere vertici e topologia
 * - Inizializzazione vertex buffer object per inviare i singoli vertici
 * - Preparazione tipi di dati da leggere
 * - Chiamata del program con glUseProgram per chiamare i vari shader
 *
 * Vertex buffer, Element buffer per la topologia
 * Rappresentazione elemento geometrico è visibile se la normale dell'elemento è diretta verso la camera
 *  ovvero i vertici sono inseriti in verso antiorario, come vertici (non come valori)
 *
 * Generazione del Vertex Buffer Object e Vertex Array Object
 * Esempio un oggettto è dato da un insieme di vertici, elementi (topologia)
 * Generare vbo e vao tramite funzioni predefinite semplifica chiamate a runtime
 */

void refreshWindowSize(GLFWwindow *window, int width, int height);

void initializeGLFW();

bool setupWindowEnvironment();
bool setupOfflineRendering();

void compileShaders();

void loadObjects();
void loadCards();

void generateObjects(Mesh& mesh);
void prepareScreenForOfflineRendering();
void prepareCardRendering();

void prepareSceneLights();

void setupRenderVariables();
void render();
void renderCardsInLoop(unsigned int& pIndex, size_t& i, bool& hasSelectedCard, double x, double y);
void renderShadowMap();
void renderSceneObjects();
void renderCards();
void renderCardsOnTable();

void cleanMemory();

#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(const char *file, int line);

static int initialise() {
   initializeGLFW();

   if (!setupWindowEnvironment()) {
      std::cout << "Error GLFW_INITIALIZATION: cannot initialize window." << std::endl;
      return EXIT_FAILURE;
   }

   if (!setupOfflineRendering()) {
      std::cout << "Error OFFLINE_RENDERING_INITIALIZATION: cannot create framebuffer." << std::endl;
      return EXIT_FAILURE;
   }

   MAX_SIZE_T_VALUE = std::numeric_limits<size_t>::max();

   compileShaders();
   loadObjects();

   // Chiamate di GLAD e di GLFW: creazione di Render Loop (infinito, finisce quando esce dalla finestra)
   render();
   cleanMemory();

   glfwTerminate();

   return 0;
}

#endif //GL_LOADER_H
