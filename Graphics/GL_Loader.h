#ifndef GL_LOADER_H
#define GL_LOADER_H

#include "glad.h"
#include "glfw3.h"
#include "GLSL.h"

#include <iostream>

// TODO add points as Float3
float vertices[] {-0.5f, -0.5f, 0.0f,
                  0.5f, -0.5f, 0.0f,
                  0.0f, -0.5f, 0.0f};
//In coordinate NDC da inviare allo shader

//Vertex buffer, Element buffer per la topologia
// Rappresentazione elemento geometrico è visibile se la normale dell'elemento è diretta verso la camera
//  ovvero i vertici sono inseriti in verso antiorario, come vertici (non come valori)

const unsigned int WIDTH = 640;
const unsigned int HEIGHT = 480;

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

static int initialise() {
   // Inzializzazione di OpenGL per il render
   glfwInit();
   // Setup versione da utilizzare
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   // Creazione finestra, con nome, monitor da assegnare e finestre da cui dipendere
   GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cotecchio Game", nullptr, nullptr);

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

   // Creazione dello shader (vertex o fragment)
   // VERTEX SHADER
   // Restituisce GL unsigned int, indice/puntatore dell'oggetto vertex shader creato dalla GPU
   // Operazione su valori binari, invia chiamata sulla scheda grafica
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   // Variabili per il controllo di errori
   int report;
   char infoLog[512];

   // Assegnazione codice allo shader (handle), assegnazione char* (codice GLSL da compilare)
   glShaderSource(vertexShader, 1, &vsSource, nullptr);
   // Compilazione shader
   glCompileShader(vertexShader);

   // Controllo errori di compilazione - controllo di un handle per ottenere informazioni sulla compilazione
   // IV information value? restituisce in report il valore dello stato
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      // Errore nella compilazione
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_VERTEX: " << infoLog << std::endl;
   }

   // FRAGMENT SHADER
   // Restituisce GL unsigned int, indice dell'oggetto fragment shader creato dalla GPU
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fsSource, nullptr);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_FRAGMENT: " << infoLog << std::endl;
   }

   // Creazione contenitore (program), rappresenta la pipeline di rendering (nel senso delle possibilità programmabili dall'utente)
   GLuint shaderProgram = glCreateProgram();

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

   // Generazione del Vertex Buffer Object e Vertex Array Object
   // Esempio un oggettto è dato da un insieme di vertici, elementi (topologia)
   // Generare vbo e vao tramite funzioni predefinite semplifica chiamate a runtime

   GLuint vbo; // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
   GLuint vao; // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto

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
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Imposta il modo di interpretare i dati ottenuti dal buffer, il quale ottiene i dati dal vettore
   // Assegnare attributi a partire da determinati dati, cerca dati nella LOCATION  definita nella GLSL
   // Stride, in termini di byte: size in byte di un gruppo di dati da analizzare
   /* 0        1        2
    * x y z    x y z    x y z
    * Nelle varie posizioni saranno salvate le informazioni
    * Offset di memoria, ovvero punto zona di memoria per leggere i dati, poichè a priori non nota (potrebbero esserci dati in piu da non dover leggere
    * x y z u v  x y ... offset 2
    */
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (GLvoid*)0);
   // Abilita gli attributi passatigli
   glEnableVertexAttribArray(0);

   // Imposta il nuovo buffer a 0, ovvero slega il bind dall'array (per evitare di sovrascrivere)
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // Unbind del VAO precedentemente assegnato per evitare sovrascritture non volute
   glBindVertexArray(0);

   // Chiamate di GLAD e di GLFW
   //Creazione di Render Loop (infinito, finisce quando esce dalla finestra)
   while (!glfwWindowShouldClose(window)) { // semmai la finestra dovesse chiudersi
      // Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
      // In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
      pollInput(window);

      glClearColor(0.2f, 0.4f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(shaderProgram);
      // Caricare vertexArrayObject interessato
      glBindVertexArray(vao);
      // Chamata di disegno della primitiva
      glDrawArrays(GL_TRIANGLES, 0, 3);

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

#endif //GL_LOADER_H
