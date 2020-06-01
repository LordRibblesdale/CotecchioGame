#ifndef GL_LOADER_H
#define GL_LOADER_H

#include "glad.h"

#ifdef _WIN32
#include "glfw3.h"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif

#include "Shaders.h"
#include "../Vector/Float2.h"
#include "../Vector/Float3.h"
#include "../Utilities/Color.h"
#include "../Matrix/SquareMatrix.h"
#include "../Matrix/StandardMatrix.h"
#include "../Utilities/Camera.h"
#include "../Model/Object.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../IO/stb-master/stb_image.h"

#include <iostream>
#include <vector>
#include <cmath>

//Vertex buffer, Element buffer per la topologia
// Rappresentazione elemento geometrico è visibile se la normale dell'elemento è diretta verso la camera
//  ovvero i vertici sono inseriti in verso antiorario, come vertici (non come valori)

const unsigned int WIDTH = 940;
const unsigned int HEIGHT = 560;

GLFWwindow* window = nullptr;
GLuint shaderProgram = 0;
GLuint vbo = 0; // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
GLuint vao = 0; // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
GLuint vbo2 = 0;
GLuint vao2 = 0;
// Creazione texture
GLuint texture1 = 0;
GLuint textureUniform = 0;
GLuint matrixUniform = 0;

void refreshWindowSize(GLFWwindow *vindow, int width, int height) {
   // La Callback prevere azioni sull'immagine, per poi riproiettarla tramite glViewport
   // glViewport è la funzione per la trasformazione da NDC a Screen
   glViewport(0, 0, width, height);
}

void pollInput(GLFWwindow *window) {
   // Funzione per l'input, esempio via tastiera
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   } /*else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      for (int i = 0; i < 3; ++i) {
         vertices[3*i] -= 0.1;
      }
   } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      for (int i = 0; i < 3; ++i) {
         vertices[3*i] += 0.1;
      }
   } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      for (int i = 0; i < 3; ++i) {
         vertices[3*i+1] -= 0.1;
      }
   } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      for (int i = 0; i < 3; ++i) {
         vertices[3*i+1] -= 0.1;
      }
   }
   */
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

void checkInputs();
void initialiseGLFW();
void compileShaders();
void createBackground(float* attributes);
void createCharacter(float* attributes);

static int initialise() {
   initialiseGLFW();

   // Creazione finestra, con nome, monitor da assegnare e finestre da cui dipendere
   window = glfwCreateWindow(WIDTH, HEIGHT, "Cotecchio Game", nullptr, nullptr);

   if (!window) {
      // Controllo in caso di errore di inizializzazione e pulizia del programma annessa
      std::cout << "Error INITIALISATION: window cannot be initialised.";
      glfwTerminate();
      return -1;
   }

   // Crea un contesto (link con il thread) di OpenGL alla finestra (permette l'impostazione della macchina di stato)
   glfwMakeContextCurrent(window);
   // Chiamare determinate callbacks per ogni azione (funzioni da richiamare in un certo evento)
   glfwSetWindowSizeCallback(window, refreshWindowSize);

   if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      // Controllo in caso di errore di caricamento puntatori alle funzioni della scheda video
      std::cout << "Error LOADING_GL: libraries cannot be called";
      glfwTerminate();
      return -1;
   }

   compileShaders();

   // Generazione del Vertex Buffer Object e Vertex Array Object
   // Esempio un oggettto è dato da un insieme di vertici, elementi (topologia)
   // Generare vbo e vao tramite funzioni predefinite semplifica chiamate a runtime

   // E' possibile attribuire durante il ciclo il colore, tramite l'uniform vec4
   // Gathering variabile uniform del pixel shader che risiede nel programma, ma non si accede tramite puntatore
   //    -> Accesso tramite  (poichè puntatore non generato nello shader)
   //GLuint colorUniformLocation = glGetUniformLocation(shaderProgram, "color");  // Puntatore alla variabile

   float attributes[] {-1, 1, 0, 0, 1,
                       -1, -1, 0, 0, 0,
                       1, -1, 0, 1, 0,

                       1, -1, 0, 1, 0,
                       1, 1, 0, 1, 1,
                       -1, 1, 0, 0, 1
   };

   /* Generazione degli indici
 * Generazione del VertexArray e VertexBuffer
 * Bind del VAO e VBO
 * Creazione dati e invio al buffer
 * Impostazione lettura dati
 * Scollegare bind
 */

   createBackground(attributes);
   createCharacter(attributes);

   matrixUniform = glGetUniformLocation(shaderProgram, "scale");

   // Chiamate di GLAD e di GLFW
   //Creazione di Render Loop (infinito, finisce quando esce dalla finestra)
   while (!glfwWindowShouldClose(window)) { // semmai la finestra dovesse chiudersi
      // Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
      // In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
      checkInputs();

      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      // Pulizia buffer colore e depth
      glClear(GL_COLOR_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(shaderProgram);

      // Scrivere nella location della variabile i valori del colore da assegnare al pixel;
      // Essendo macchina di stato, bisogna ricordare che la posizione influisce sull'azione delle chiamate
      // Quindi attenzione al posizionamento delle chiamate di modifica stato

      // Caricare vertexArrayObject interessato
      glBindVertexArray(vao);

      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE0);
      glUniform1i(textureUniform, 0);

      float id[16] {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

      //glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, Transform::scaleMatrix4(1, 1, 1).getArray());
      glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, id);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      /*
      glBindVertexArray(vao2);
      glUniform1i(textureUniform, 0);
      glBindTexture(GL_TEXTURE_2D, texture2);
      glActivateTexture(GL_TEXTURE1);
      glDrawArrays(GL_TRIANGLES, 0, 6);
       */

      //Necessità di modificare il buffer prima di inviarlo
      // prima, modifica il buffer B (sul successivo)
      // Crea multipli frame buffers per aggiornare i pixel, (double buffer, triple buffer, area di memoria per salvare framebuffer attuale e successivo), swap al frame buffer preparatorio)
      glfwSwapBuffers(window);
      // Sostituisce questo buffer con quello successivo, visualizzando quello già riempito
      // Cambia frame buffer su cui lavorare
      glfwPollEvents();
      // Controlla tutti gli eventi in background (qualunque) OBBLIGATORIO
   }

   // Liberazione della memoria
   glDeleteBuffers(1, &vbo);
   glDeleteVertexArrays(1, &vao);
   glDeleteProgram(shaderProgram);
   glfwTerminate();

   return 0;
}

void checkInputs() {
   pollInput(window);
}

void initialiseGLFW() {
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

void compileShaders() {
   // Creazione dello shader (vertex o fragment)
   // VERTEX SHADER
   // Restituisce GL unsigned int, indice/puntatore dell'oggetto vertex shader creato dalla GPU
   // Operazione su valori binari, invia chiamata sulla scheda grafica
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   // Variabili per il controllo di errori
   int report;
   char infoLog[512];

   //TODO optimise here
   // Attenzione al puntatore, da eliminare appena utilizzato, andrà sempre cancellato
   std::string source;
   loadShader(source, "Graphics/vertex_texture.glsl");
   char* charSource(const_cast<char *>(source.c_str()));

   if (!charSource) {
      std::cout << "Error VERTEX_FILE_IMPORT" << std::endl;
   }

   // Assegnazione codice allo shader (handle), assegnazione char* (codice GLSL da compilare)
   glShaderSource(vertexShader, 1, &charSource, nullptr);
   // Compilazione shader
   glCompileShader(vertexShader);

   // Controllo errori di compilazione - controllo di un handle per ottenere informazioni sulla compilazione
   // IV information value? restituisce in report il valore dello stato
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      // Errore nella compilazione
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_VERTEX" << std::endl;
   }

   loadShader(source, "Graphics/fragment_texture.glsl");
   charSource = const_cast<char *>(source.c_str());

   if (!charSource) {
      std::cout << "Error FRAGMENT_FILE_IMPORT" << std::endl;
   }

   // FRAGMENT SHADER
   // Restituisce GL unsigned int, indice dell'oggetto fragment shader creato dalla GPU
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &charSource, nullptr);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_FRAGMENT: " << infoLog << std::endl;
   }

   // Creazione contenitore (program), rappresenta la pipeline di rendering (nel senso delle possibilità programmabili dall'utente)
   shaderProgram = glCreateProgram();

   // Aggiunta del programma dei vari shader
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);

   // Link del programma, unisce le unità programmabili (shader)
   glLinkProgram(shaderProgram);

   // Controllo del link programma, status
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &report);

   if (!report) {
      glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_LINK_PROGRAM: " << infoLog << std::endl;
   }

   // Pulizia memoria dopo la compilazione e link
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

void createBackground(float* attributes) {
   // Genera il Vertex Array Object
   glGenVertexArrays(1, &vao);
   // Genera il Vertex Buffer Object
   glGenBuffers(1, &vbo);
   // Chiamata per collegare un tipo di buffer noto agli attributi di vertice, l'indice dell'area di memoria creata va intesa come arraybuffer
   // Bind all'inizio delle operazioni riferite al VAO
   // Binding: ogni chiamata di tipo ARRAY_BUFFER sarà assegnata all'ultimo bind assegnato
   glBindVertexArray(vao);

   // Copia dati nell'array, inizializzando la memoria nel punto bindato del buffer (prima solo indice, VBO)
   // GL_STATIC_DRAW imposta punti che non verranno modificati ma solo disegnati ogni volta
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5*6, attributes, GL_DYNAMIC_DRAW);

   // Imposta il modo di interpretare i dati ottenuti dal buffer, il quale ottiene i dati dal vettore
   // Assegnare attributi a partire da determinati dati, cerca dati nella LOCATION  definita nella GLSL
   // Stride, in termini di byte: size in byte di un gruppo di dati da analizzare
   /* 0        1        2
    * x y z    x y z    x y z
    * Nelle varie posizioni saranno salvate le informazioni
    * Offset di memoria, ovvero punto zona di memoria per leggere i dati, poichè a priori non nota (potrebbero esserci dati in piu da non dover leggere
    * x y z u v  x y ... offset 2
    */
   // DEFINITA nella scrittura dello shader
   // Stride definisce l'intero vettore, l'offset è da dove iniziare a leggere
   // Il valore 3 dice quanti vertici
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(GLfloat), (GLvoid*) 0);
   // Lettura del buffer, con un offset di lettura dei 3 valori GL_FLOAT di 3 posizioni;
   // Abilita gli attributi passatigli
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(GLfloat), (GLfloat*) (3* sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   // Imposta il nuovo buffer a 0, ovvero slega il bind dall'array (per evitare di sovrascrivere)
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // Unbind del VAO precedentemente assegnato per evitare sovrascritture non volute
   glBindVertexArray(0);

   // Creazione allocazione memoria texture
   glGenTextures(1, &texture1);
   // Bind della texture
   glBindTexture(GL_TEXTURE_2D, texture1);
   // Impostare come applicare texture su s e t
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
   // Impostare come comportarsi con dimensioni più o meno piccole in base alla distanza (es usando mipmap)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Acquisizione texture
   int width, height, channels;
   // Ottenimento matrice dei pixel (1 byte, 8 bit per canale) per valori da 0 a 255, come i PNG 8bit per channel, tramite stb_load)
   // ATTENZIONE nella lettura della texture: In base all'orientamento dell'oggetto, bisogna leggere il file in modo diverso
   // Es: oggetto dal basso verso l'alto, e le immagini dall'alto verso il basso, per un corretto riempimento del buffer
   stbi_set_flip_vertically_on_load(true); // Per leggere il file nell'ordine corretto

   unsigned char* data = stbi_load("platform.png", &width, &height, &channels, 0);

   if (data) {
      // Analisi dell'immagine, come elaborarla e come farla studiare dalla GPU,
      //   con informazioni su livelli, canali (es RGBA), dimensioni immagine, formato e formato interno (che dovranno coincidere)
      //   tipo pixel (GL_UNSIGNED_BYTE), array di pixel
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      // Creazione della mipmap della texture bindata
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Error IMG_LOAD: image not loaded." << std::endl;
   }

   stbi_image_free(data);
}

void createCharacter(float* attributes) {
   glGenVertexArrays(1, &vao2);
   glGenBuffers(1, &vbo2);

   glBindVertexArray(vao2);
   glBindBuffer(GL_ARRAY_BUFFER, vbo2);

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5*6, attributes, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   /* Generazione texture
    * Assegnamento indirizzo
    * Bind texture
    * Impostazione trasformazioni texture
    *    - Sul minifyier e magnifyier
    *    - Sulla trasformazione U e V
    *    - Sulla mipmap
    * Caricamento texture
    * Pulizia texture dopo caricamento
    */

   GLuint texture2;
   glGenTextures(1, &texture2);
   glBindTexture(GL_TEXTURE_2D, texture2);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   int width, height, channels;

   unsigned char* data = stbi_load("charzera.png", &width, &height, &channels, 0);

   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Error IMG_LOAD: image not loaded." << std::endl;
   }

   stbi_image_free(data);
   glBindTexture(GL_TEXTURE_2D, 0);

   // Richiesta della posizione della texture
   // Ricerca dello uniform nello shaderProgram necessario, laddove serve caricarlo
   textureUniform = glGetUniformLocation(shaderProgram, "texture1");
}

#endif //GL_LOADER_H
