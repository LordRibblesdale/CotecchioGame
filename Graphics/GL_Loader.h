#ifndef GL_LOADER_H
#define GL_LOADER_H

#include "glad.h"

#ifdef _WIN32
#include "glfw3.h"
#include "../Model/Mesh.h"

#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif

//#include "Settings.h"

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

void cursorPositionCallBack(GLFWwindow* window, double xPos, double yPos);

void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset);

void pollInput(GLFWwindow *window);

//------------------------------------------------------------------------------//

void initializeGLFW();

bool setupWindowEnvironment();

bool setupOfflineRendering();

void compileShaders();

void loadObjects();

void generateObjects(const Mesh& mesh);

void prepareScreenForOfflineRendering();

void render();

void cleanMemory();

static int initialise() {
   // TODO add function for setting up all libraries and OpenGL (from settings file)
   initializeGLFW();

   if (!setupWindowEnvironment()) {
      std::cout << "Error GLFW_INITIALIZATION: cannot initialize window." << std::endl;
      return EXIT_FAILURE;
   }

   if (!setupOfflineRendering()) {
      std::cout << "Error OFFLINE_RENDERING_INITIALIZATION: cannot create framebuffer." << std::endl;
      return EXIT_FAILURE;
   }

   compileShaders();
   loadObjects();

   //TODO create method for specific OpenGL features
   glEnable(GL_MULTISAMPLE);

   // Chiamate di GLAD e di GLFW: creazione di Render Loop (infinito, finisce quando esce dalla finestra)
   render();
   cleanMemory();

   glfwTerminate();

   return 0;
}

#endif //GL_LOADER_H
