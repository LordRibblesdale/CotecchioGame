#ifndef GL_LOADER_H
#define GL_LOADER_H

#include "glad.h"

#ifdef _WIN32
#include "glfw3.h"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif

#include "../Vector/Float2.h"
#include "../Vector/Float3.h"
#include "../Utilities/Color.h"
#include "../Matrix/SquareMatrix.h"
#include "../Matrix/StandardMatrix.h"
#include "../Utilities/Camera.h"
#include "../Model/Model.h"

#include <vector>
#include <cmath>

#include "Settings.h"

const std::string TABLE_ASSETS_LOCATION("DATA_ASSETS\\Table_OBJ\\");

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

void fillModelMemory(const Model& model);

void initializeGLFW();

bool setUpWindowEnvironment();

void compileShaders();

void loadObjects();

void generateObjects(const Mesh& mesh);

void render();

void cleanMemory();

static int initialise() {
   // TODO add function for setting up all libraries and OpenGL (from settings file)
   initializeGLFW();

   if (!setUpWindowEnvironment()) {
      return EXIT_FAILURE;
   }

   compileShaders();
   loadObjects();

   // TODO prepare textures
   //setUpTexture(texture1);    // Creazione texture
   //loadTexture(texture1, "img.png");

   /* Richiesta della posizione della texture
    * Ricerca dello uniform nello shaderProgram necessario, laddove serve caricarlo
   GLuint textureUniform = glGetUniformLocation(shaderProgram, "texture1");

    * Assegnazione e modifica uniform a prescindere
    * Uso glUseProgram per assegnare texture
   glUseProgram(shaderProgram);
    * Assegnazione valore della texture a uno specifico canale di OpenGL
    * Canali limitati, massimo un certo numero di texture contemporaneamente

   glUniform1i(textureUniform, 0);

   glUseProgram(0);
   */

   //TODO create method for specific OpenGL features
   glEnable(GL_MULTISAMPLE);

   /* Chiamate di GLAD e di GLFW
    * Creazione di Render Loop (infinito, finisce quando esce dalla finestra)
    */

   render();
   cleanMemory();

   glfwTerminate();

   return 0;
}

#endif //GL_LOADER_H
