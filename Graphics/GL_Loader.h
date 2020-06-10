#ifndef GL_LOADER_H
#define GL_LOADER_H

#include "glad.h"

#ifdef _WIN32
#include "glfw3.h"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "../IO/stb-master/stb_image.h"

#include "Shaders.h"
#include "../Vector/Float2.h"
#include "../Vector/Float3.h"
#include "../Utilities/Color.h"
#include "../Matrix/SquareMatrix.h"
#include "../Matrix/StandardMatrix.h"
#include "../Utilities/Camera.h"
#include "../Model/Light/OmniLight.h"
#include "../Model/Model.h"

#include <iostream>
#include <vector>
#include <cmath>

//In coordinate NDC da inviare allo shader
std::vector<Model> objects;

OmniLight light(Float3(2, -3, 0), Color(1, 1, 1), 5);

/* Vertex buffer, Element buffer per la topologia
 * Rappresentazione elemento geometrico è visibile se la normale dell'elemento è diretta verso la camera
 *  ovvero i vertici sono inseriti in verso antiorario, come vertici (non come valori)
 */

int WIDTH = 960;
int HEIGHT = 540;
float aspectRatio = static_cast<float>(HEIGHT) / static_cast<float>(WIDTH);
GLFWwindow* window = nullptr;
Camera cam(std::move(Float3(0, -5, 0)), std::move(Float3(0, 0, 0)), std::move(Float3(0, 0, 1)),
           0.4f, 10000, 35, aspectRatio);

/* Generazione del Vertex Buffer Object e Vertex Array Object
 * Esempio un oggettto è dato da un insieme di vertici, elementi (topologia)
 * Generare vbo e vao tramite funzioni predefinite semplifica chiamate a runtime
 */

GLuint colorOnlyShaderProgram;

std::vector<GLuint> vertexArrayObjects;   // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
std::vector<GLuint> vertexBufferObjects;  // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
std::vector<GLuint> elementBufferObjects;

GLuint texture1;

void refreshWindowSize(GLFWwindow *window, int width, int height) {
   /* La Callback prevere azioni sull'immagine, per poi riproiettarla tramite glViewport
    * glViewport è la funzione per la trasformazione da NDC a Screen
    */
   glfwGetWindowSize(window, &WIDTH, &HEIGHT);
   aspectRatio = static_cast<float>(HEIGHT) / static_cast<float>(WIDTH);
   cam.setAspectRatio(aspectRatio);
   glViewport(0, 0, width, height);
}

void cursorPositionCallBack(GLFWwindow* window, double xPos, double yPos) {
   if (xPos != 0) {
      float angle = cam.getSensibility() * atanf(xPos);
      std::cout << angle << std::endl;

      Float3 tmp(cam.getLookAt()-cam.getEye());

      Float3 rotate(Rotation::axisZRotateVertex3(tmp, -angle));
      rotate += cam.getEye();

      cam.setLookAt(rotate);
   }

   if (yPos != 0) {
      /*
      float angle = cam.getSensibility() * 0.01f * atanf(xPos);

      Float3 tmp(cam.getLookAt().crossProduct(cam.getEye()));

      Float3 rotate(Rotation::axisZRotateVertex3(tmp, -angle));

      cam.setLookAt(rotate);
       */
   }

   glfwSetCursorPos(window, 0, 0);
}

void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset) {
   if (yOffset > 0) {
      cam.setNear(cam.getNear()+0.5f);
   } else if (yOffset < 0) {
      cam.setNear(cam.getNear()-0.5f);
   }
}

void pollInput(GLFWwindow *window) {
   //TODO add time-based input
   float speed = 0.01f + ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 0.04f : 0);

   // Funzione per l'input, esempio via tastiera
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      Float3 tmp(-(cam.getEye().getY() - cam.getLookAt().getY()), cam.getEye().getX() - cam.getLookAt().getX(), 0);

      cam.setEye(cam.getEye() - Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
      cam.setLookAt(cam.getLookAt() - Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
   } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      Float3 tmp(-(cam.getEye().getY() - cam.getLookAt().getY()), cam.getEye().getX() - cam.getLookAt().getX(), 0);

      cam.setEye(cam.getEye() + Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
      cam.setLookAt(cam.getLookAt() + Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
   } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      Float3 tmp(cam.getEye() - cam.getLookAt());

      cam.setEye(cam.getEye() + Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
      cam.setLookAt(cam.getLookAt() + Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
   } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      Float3 tmp(cam.getEye() - cam.getLookAt());

      cam.setEye(cam.getEye() - Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
      cam.setLookAt(cam.getLookAt() - Float3(speed * tmp.getX(), speed * tmp.getY(), 0));
   }
}

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
 */

void initializeGLFW() {
   // Inzializzazione di OpenGL per il render
   glfwInit();
   // Setup versione da utilizzare
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   #ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   #endif
}

void setUpWindowEnvironment() {
   // Creazione finestra, con nome, monitor da assegnare e finestre da cui dipendere
   window = glfwCreateWindow(WIDTH, HEIGHT, "Cotecchio Game", nullptr, nullptr);

   if (!window) {
      // Controllo in caso di errore di inizializzazione e pulizia del programma annessa
      std::cout << "Error INITIALISATION: window cannot be initialised.";
      glfwTerminate();
      throw(EXIT_FAILURE);
   }

   // Crea un contesto (link con il thread) di OpenGL alla finestra (permette l'impostazione della macchina di stato)
   glfwMakeContextCurrent(window);
   // Chiamare determinate callbacks per ogni azione (funzioni da richiamare in un certo evento)
   glfwSetWindowSizeCallback(window, refreshWindowSize);

   // Imposto input tramite mouse
   glfwSetCursorPosCallback(window, cursorPositionCallBack);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPos(window, 0, 0);

   glfwSetScrollCallback(window, scrollCallBack);

   if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      // Controllo in caso di errore di caricamento puntatori alle funzioni della scheda video
      std::cout << "Error LOADING_GL: libraries cannot be called";
      glfwTerminate();

      //TODO fix here
      throw(EXIT_FAILURE);
   }
}

void compileShaders() {
   /* Creazione dello shader (vertex o fragment)
 * VERTEX SHADER
 * Restituisce GL unsigned int, indice/puntatore dell'oggetto vertex shader creato dalla GPU
 * Operazione su valori binari, invia chiamata sulla scheda grafica
 */
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   // Variabili per il controllo di errori
   int report;
   char infoLog[512];

   std::string source;
   loadShader(source, "Graphics/Shader Files/vertex.glsl");
   char* charSource(const_cast<char *>(source.c_str()));

   if (!charSource) {
      std::cout << "Error VERTEX_FILE_IMPORT" << std::endl;
   }

   // Assegnazione codice allo shader (handle), assegnazione char* (codice GLSL da compilare)
   glShaderSource(vertexShader, 1, &charSource, nullptr);
   // Compilazione shader
   glCompileShader(vertexShader);

   /* Controllo errori di compilazione - controllo di un handle per ottenere informazioni sulla compilazione
    * IV information value? restituisce in report il valore dello stato
    */
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      // Errore nella compilazione
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_VERTEX" << std::endl;
   }

   loadShader(source, "Graphics/Shader Files/fragment.glsl");
   charSource = const_cast<char *>(source.c_str());

   if (!charSource) {
      std::cout << "Error FRAGMENT_FILE_IMPORT" << std::endl;
   }

   /* FRAGMENT SHADER
    * Restituisce GL unsigned int, indice dell'oggetto fragment shader creato dalla GPU
    */
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &charSource, nullptr);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_FRAGMENT: " << infoLog << std::endl;
   }

   // Creazione contenitore (program), rappresenta la pipeline di rendering (nel senso delle possibilità programmabili dall'utente)
   colorOnlyShaderProgram = glCreateProgram();

   // Aggiunta del programma dei vari shader
   glAttachShader(colorOnlyShaderProgram, vertexShader);
   glAttachShader(colorOnlyShaderProgram, fragmentShader);

   // Link del programma, unisce le unità programmabili (shader)
   glLinkProgram(colorOnlyShaderProgram);

   // Controllo del link programma, status
   glGetProgramiv(colorOnlyShaderProgram, GL_LINK_STATUS, &report);

   if (!report) {
      glGetProgramInfoLog(colorOnlyShaderProgram, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_LINK_PROGRAM: " << infoLog << std::endl;
   }

   // Pulizia memoria dopo la compilazione e link
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

void generateObjects(const Mesh& mesh) {
   /* E' possibile attribuire durante il ciclo il colore, tramite l'uniform vec4
   * Gathering variabile uniform del pixel shader che risiede nel programma, ma non si accede tramite puntatore
   *    -> Accesso tramite  (poichè puntatore non generato nello shader)
   */
   GLuint vao;
   GLuint vbo;
   GLuint ebo;

   // Genera il Vertex Array Object
   glGenVertexArrays(1, &vao);
   // Genera il Vertex Buffer Object
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);
   /* Chiamata per collegare un tipo di buffer noto agli attributi di vertice, l'indice dell'area di memoria creata va intesa come arraybuffer
    * Bind all'inizio delle operazioni riferite al VAO
    * Binding: ogni chiamata di tipo ARRAY_BUFFER sarà assegnata all'ultimo bind assegnato
    */
   glBindVertexArray(vao);

   /* Copia dati nell'array, inizializzando la memoria nel punto bindato del buffer (prima solo indice, VBO)
    * GL_STATIC_DRAW imposta punti che non verranno modificati ma solo disegnati ogni volta
    */
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao);

   //TODO check resize and memory allocation
   //TODO optimise multiple array usage
   //TODO change if there is another type of object
   unsigned int size = mesh.getVertices().size();
   float attributes[3*size];

   int index = 0;
   for (int i = 0; i < size; ++i) {
      attributes[index] = mesh.getVertices().at(i).getX();
      attributes[++index] = mesh.getVertices().at(i).getY();
      attributes[++index] = mesh.getVertices().at(i).getZ();
   }

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, attributes, GL_DYNAMIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getIndices().size(), mesh.getIndices().data(), GL_DYNAMIC_DRAW);

   /* Imposta il modo di interpretare i dati ottenuti dal buffer, il quale ottiene i dati dal vettore
    * Assegnare attributi a partire da determinati dati, cerca dati nella LOCATION  definita nella GLSL
    * Stride, in termini di byte: size in byte di un gruppo di dati da analizzare
    * 0        1        2
    * x y z    x y z    x y z
    * Nelle varie posizioni saranno salvate le informazioni
    * Offset di memoria, ovvero punto zona di memoria per leggere i dati, poichè a priori non nota (potrebbero esserci dati in piu da non dover leggere
    * x y z u v  x y ... offset 2
    * DEFINITA nella scrittura dello shader
    * Stride definisce l'intero vettore, l'offset è da dove iniziare a leggere
    * Il valore 3 dice quanti vertici
    */
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid*) 0);
   /* Lettura del buffer, con un offset di lettura dei 3 valori GL_FLOAT di 3 posizioni;
    * Abilita gli attributi passatigli
    */
   glEnableVertexAttribArray(0);


   // Imposta il nuovo buffer a 0, ovvero slega il bind dall'array (per evitare di sovrascrivere)
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // Unbind del VAO precedentemente assegnato per evitare sovrascritture non volute
   glBindVertexArray(0);

   vertexArrayObjects.emplace_back(vao);
   vertexBufferObjects.emplace_back(vbo);
   elementBufferObjects.emplace_back(ebo);
}

void draw() {

}

static int initialise() {
   initializeGLFW();
   setUpWindowEnvironment();
   compileShaders();

   for (const Model& model : objects) {
      for (const Mesh& mesh : model.getMeshes()) {
         generateObjects(mesh);
      }
   }

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

   // Collezione indici per inviare dati allo shader
   GLuint projectionMatrixUniform = glGetUniformLocation(colorOnlyShaderProgram, "projection");
   GLuint viewMatrixUniform = glGetUniformLocation(colorOnlyShaderProgram, "view");
   GLuint modelMatrixUniform = glGetUniformLocation(colorOnlyShaderProgram, "model");

   //GLuint lightPosUniform = glGetUniformLocation(shaderProgram, "lightPos");
   //GLuint lightColorUniform = glGetUniformLocation(shaderProgram, "lightColor");
   //GLuint lightIntensity = glGetUniformLocation(shaderProgram, "lightIntensity");

   //GLuint ambientCoefficient = glGetUniformLocation(shaderProgram, "ambientCoefficient");
   //GLuint diffusiveCoefficient = glGetUniformLocation(shaderProgram, "diffusiveCoefficient");
   //GLuint specularCoefficient = glGetUniformLocation(shaderProgram, "specularCoefficient");
   //GLuint specularAlpha = glGetUniformLocation(shaderProgram, "specularAlpha");
   //GLuint eyePosition = glGetUniformLocation(shaderProgram, "eye");

   GLuint color = glGetUniformLocation(colorOnlyShaderProgram, "inColor");

   /* Chiamate di GLAD e di GLFW
    * Creazione di Render Loop (infinito, finisce quando esce dalla finestra)
    */
   while (!glfwWindowShouldClose(window)) {  // semmai la finestra dovesse chiudersi
      /* Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
       * In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
       */
      pollInput(window);

      glClearColor(0.1, 0.9, 0.1, 1.0f);
      // Pulizia buffer colore e depth
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Abilito il depth test per il check della profondità per la stampa a video degli oggetti
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(colorOnlyShaderProgram);

      //glBindTexture(GL_TEXTURE_2D, texture1);
      // Attivazione canale texture (Texture Unit), per poter utilizzare il canale (che dentro è presente una texture)
      //glActiveTexture(GL_TEXTURE0);

      /* Scrivere nella location della variabile i valori del colore da assegnare al pixel;
       * Essendo macchina di stato, bisogna ricordare che la posizione influisce sull'azione delle chiamate
       * Quindi attenzione al posizionamento delle chiamate di modifica stato
       */

      SquareMatrix p(std::move(Projection::onAxisFOV2ClipProjectiveMatrix(cam)));
      SquareMatrix v(std::move(cam.world2ViewMatrix()));
      SquareMatrix m(std::move(Transform::tranScalaRotoMatrix4(0, 0, 0, 0.25, 0.25, 0.25, 0, 0, 0)));

      glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, p.getArray());
      glUniformMatrix4fv(viewMatrixUniform, 1, GL_TRUE, v.getArray());
      glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, m.getArray());

      //light.setOrigin(light.getOrigin() + Float3(0.05f*sinf(glfwGetTime()), 0, 0));

      //glUniform3f(lightPosUniform, light.getOrigin().getX(), light.getOrigin().getY(), light.getOrigin().getZ());
      //glUniform3f(lightColorUniform, light.getColor().getRed(), light.getColor().getGreen(), light.getColor().getBlue());
      //TODO manage falloff
      //glUniform3f(lightIntensity, 1.0f, 1.0f, 1.0f);

      //glUniform3f(ambientCoefficient, 0, 0, 0);
      //glUniform3f(diffusiveCoefficient, 0.8, 0.8f, 0.8);
      //glUniform3f(specularCoefficient, 0.5, 0.5, 0.5);
      //glUniform1f(specularAlpha, 110);

      //glUniform3f(eyePosition, cam.getEye().getX(), cam.getEye().getY(), cam.getEye().getZ());

      // Caricare vertexArrayObject interessato

      glUniform4f(color, 0.3, 0.3, 0.3, 1);

      for (unsigned int& vertexArrayObject : vertexArrayObjects) {
         glBindVertexArray(vertexArrayObject);
         // Chamata di disegno della primitiva
         glDrawArrays(GL_TRIANGLES, 0, 3);
      }

      // Disabilito il Depth Test per poter aggiungere varie informazioni o effetti a schermo
      glDisable(GL_DEPTH_TEST);

      /* Necessità di modificare il buffer prima di inviarlo
       * prima, modifica il buffer B (sul successivo)
       * Crea multipli frame buffers per aggiornare i pixel, (double buffer, triple buffer,
       * area di memoria per salvare framebuffer attuale e successivo), swap al frame buffer preparatorio)
       */
      glfwSwapBuffers(window);
      /* Sostituisce questo buffer con quello successivo, visualizzando quello già riempito
       * Cambia frame buffer su cui lavorare
       */
      glfwPollEvents();
      // Controlla tutti gli eventi in background (qualunque) OBBLIGATORIO
   }

   // Liberazione della memoria
   for (int i = 0; i < vertexArrayObjects.size(); ++i) {
      glDeleteBuffers(1, &vertexBufferObjects.at(i));
      glDeleteVertexArrays(1, &vertexArrayObjects.at(i));

   }

   glDeleteProgram(colorOnlyShaderProgram);
   glfwTerminate();

   return 0;
}

#endif //GL_LOADER_H
