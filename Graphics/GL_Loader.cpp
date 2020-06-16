#include "GL_Loader.h"

#include <memory>
#include "Shaders.h"
#include "../Model/Light/SpotLight.h"
#include "../Animation/CameraRotation.h"

int WIDTH = 960;
int HEIGHT = 540;
float aspectRatio = static_cast<float>(HEIGHT) / static_cast<float>(WIDTH);

Camera camera(std::move(Float3(-15, 15, 15)), std::move(Float3(-14, 14, 14)), std::move(Float3(0, 0, 1)),
              0.4f, 10000, 35, aspectRatio);
Float3 position1(0, 10, 0);
double cameraAnimationTime;

GLFWwindow* window = nullptr;

double prevXPos, prevYPos;
double prevTime, currTime, sumTime;

std::vector<Model> objects;

SpotLight light(std::move(Float3(-14, 14, 14)), Color(1, 1, 1), 5, degree2Radiants(40), degree2Radiants(60));

GLuint colorOnlyShaderProgram;
GLuint shaderProgram;

std::vector<GLuint> vertexArrayObjects;   // Vertex Buffer Object, buffer per inviare i dettagli per dare dettagli del vertice
std::vector<GLuint> vertexBufferObjects;  // Vertex Array Object, contenitore per inserire array, vertici e topologia, usandolo come definizione logica dell'oggetto
std::vector<GLuint> elementBufferObjects;
std::vector<GLuint> textureUniforms;

bool TRANSFORM_CAMERA = false;
std::unique_ptr<CameraRotation> cameraRotation;

void refreshWindowSize(GLFWwindow *window, int width, int height) {
   /* La Callback prevere azioni sull'immagine, per poi riproiettarla tramite glViewport
    * glViewport è la funzione per la trasformazione da NDC a Screen
    */
   glfwGetWindowSize(window, &WIDTH, &HEIGHT);
   aspectRatio = static_cast<float>(HEIGHT) / static_cast<float>(WIDTH);
   camera.setAspectRatio(aspectRatio);
   glViewport(0, 0, width, height);
}

void cursorPositionCallBack(GLFWwindow *window, double xPos, double yPos) {
   float diffX = camera.getSensibility() * (xPos - prevXPos);
   float diffY = camera.getSensibility() * (yPos - prevYPos);

   if (diffX != 0) {
      float angle = atanf(diffX/100.0f);

      Float3 tmp(std::move((camera.getLookAt() - camera.getEye()).getNormalized()));
      tmp = std::move(Rotation::axisZRotateVertex3(tmp, -angle));

      camera.setYawAngle(acosf(tmp.getNormalized().getX()));

      tmp += camera.getEye();
      camera.setLookAt(tmp);

      prevXPos = xPos;
   }

   if (diffY != 0) {
      float angle = atanf(diffY/100.0f);

      Float3 tmp(std::move((camera.getLookAt() - camera.getEye()).getNormalized()));

      tmp = std::move(Rotation::axisZRotateVertex3(tmp, camera.getYawAngle()*0.5f));
      tmp = std::move(Rotation::axisYRotateVertex3(tmp, angle));
      tmp = std::move(Rotation::axisZRotateVertex3(tmp, -camera.getYawAngle()*0.5f));
      tmp += camera.getEye();

      camera.setLookAt(tmp);

      prevYPos = yPos;
   }
}

void scrollCallBack(GLFWwindow *window, double xOffset, double yOffset) {
   if (yOffset > 0) {
      camera.setNear(camera.getNear() + 0.175f);
   } else if (yOffset < 0) {
      camera.setNear(camera.getNear() - 0.175f);
   }
}

void pollInput(GLFWwindow *window) {
   //TODO add time-based input
   float speed = 0.01f + ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 0.04f : 0);

   // Funzione per l'input, esempio via tastiera
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   }

   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      float tmp = vec.getX();
      vec.setX(-vec.getY());
      vec.setY(tmp);
      camera.setEye(camera.getEye() - speed*vec);
      /*
      vec *= -1;
      camera.setLookAt(camera.getLookAt() - speed*vec);
       */
   }

   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      float tmp = vec.getX();
      vec.setX(-vec.getY());
      vec.setY(tmp);
      camera.setEye(camera.getEye() + speed*vec);
      /*
      vec *= -1;
      camera.setLookAt(camera.getLookAt() + speed*vec);
       */
   }

   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      camera.setEye(camera.getEye() + speed*vec);
      /*
      vec *= -1;
      camera.setLookAt(camera.getLookAt() + speed*vec);
       */
   }

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      camera.setEye(camera.getEye() - speed*vec);
      /*
      vec *= -1;
      camera.setLookAt(camera.getLookAt() - speed*vec);
       */
   }

   if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      cameraAnimationTime = 1;
      TRANSFORM_CAMERA = true;
   }

   //---------------------------------------------------------------------------------------------------//

   if (TRANSFORM_CAMERA) {
      double diff = currTime - prevTime;
      if (!cameraRotation) {
         cameraRotation = std::make_unique<CameraRotation>(camera.getEye(), position1, 0);
      }

      if (sumTime + diff <= cameraAnimationTime) {
         std::cout << camera.getEye().toString() << std::endl;
         camera.setEye(cameraRotation->rotateCamera(camera.getEye(), diff));

         sumTime += diff;
      } else {
         sumTime = 0;
         TRANSFORM_CAMERA = false;
      }
   }
}

//---------------------------------------------------------------------------------//

void fillModelMemory(const Model &model) {
   objects.emplace_back(model);
}

void initializeGLFW() {
   // Inzializzazione di OpenGL per il render
   glfwInit();
   // Setup versione da utilizzare
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // Attivazione multisampling
   // TODO manage this from program settings
   glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

bool setUpWindowEnvironment() {
   // Creazione finestra, con nome, monitor da assegnare e finestre da cui dipendere
   window = glfwCreateWindow(WIDTH, HEIGHT, "Cotecchio Game", nullptr, nullptr);

   if (!window) {
      // Controllo in caso di errore di inizializzazione e pulizia del programma annessa
      std::cout << "Error INITIALISATION: window cannot be initialised.";
      glfwTerminate();

      return false;
   }

   // Crea un contesto (link con il thread) di OpenGL alla finestra (permette l'impostazione della macchina di stato)
   glfwMakeContextCurrent(window);
   // Chiamare determinate callbacks per ogni azione (funzioni da richiamare in un certo evento)
   glfwSetWindowSizeCallback(window, refreshWindowSize);

   // Imposto input tramite mouse
   // TODO manage camera movement
   glfwSetCursorPosCallback(window, cursorPositionCallBack);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glfwSetScrollCallback(window, scrollCallBack);

   if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      // Controllo in caso di errore di caricamento puntatori alle funzioni della scheda video
      std::cout << "Error LOADING_GL: libraries cannot be called";
      glfwTerminate();

      return false;
   }

   return true;
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

void loadObjects() {
   // TODO setup diffusive & specular

   unsigned long verticesLenght = 0;
   unsigned long texturesLength = 0;

   for (const Model& model : objects) {
      for (const Mesh& mesh : model.getMeshes()) {
         verticesLenght += mesh.getVertices().size();
      }

      texturesLength += model.getMeshes().size();
   }

   vertexArrayObjects.reserve(verticesLenght);
   vertexBufferObjects.reserve(verticesLenght);
   textureUniforms.reserve(texturesLength);

   for (const auto& object : objects) {
      for (const auto& mesh : object.getMeshes()) {
         generateObjects(mesh);
      }

      loadTexture(textureUniforms, object.getLocation(), object.getName());
   }
}

void generateObjects(const Mesh &mesh) {
   /* E' possibile attribuire durante il ciclo il colore, tramite l'uniform vec4
   * Gathering variabile uniform del pixel shader che risiede nel programma, ma non si accede tramite puntatore
   *    -> Accesso tramite (poichè puntatore non generato nello shader)
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
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

   //TODO check resize and memory allocation
   //       optimise multiple array usage
   //       change if there is another type of object
   unsigned int size = mesh.getVertices().size();
   GLfloat attributes[3*size];

   int index = 0;
   for (int i = 0; i < size; ++i) {
      attributes[index++] = mesh.getVertices().at(i).getX();
      attributes[index++] = mesh.getVertices().at(i).getY();
      attributes[index++] = mesh.getVertices().at(i).getZ();
   }

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * size, attributes, GL_DYNAMIC_DRAW);
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
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
   /* Lettura del buffer, con un offset di lettura dei 3 valori GL_FLOAT di 3 posizioni;
    * Abilita gli attributi passatigli
    */
   glEnableVertexAttribArray(0);

   // Imposta il nuovo buffer a 0, ovvero slega il bind dall'array (per evitare di sovrascrivere)
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // Unbind del VAO precedentemente assegnato per evitare sovrascritture non volute
   glBindVertexArray(0);

   // Da de-bindare dopo poichè VAO contiene i vari bind dell'EBO, se si de-bindasse prima, il VAO non avrebbe l'EBO
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   vertexArrayObjects.emplace_back(vao);
   vertexBufferObjects.emplace_back(vbo);
   elementBufferObjects.emplace_back(ebo);
}

void render() {
   // Collezione indici per inviare dati allo shader
   GLuint projectionMatrixUniform = glGetUniformLocation(colorOnlyShaderProgram, "projection");
   GLuint viewMatrixUniform = glGetUniformLocation(colorOnlyShaderProgram, "view");
   GLuint modelMatrixUniform = glGetUniformLocation(colorOnlyShaderProgram, "model");

   GLuint lightPosUniform = glGetUniformLocation(shaderProgram, "lightPos");
   GLuint lightColorUniform = glGetUniformLocation(shaderProgram, "lightColor");
   GLuint lightIntensity = glGetUniformLocation(shaderProgram, "lightIntensity");

   GLuint ambientCoefficient = glGetUniformLocation(shaderProgram, "ambientCoefficient");
   GLuint diffusiveCoefficient = glGetUniformLocation(shaderProgram, "diffusiveCoefficient");
   GLuint specularCoefficient = glGetUniformLocation(shaderProgram, "specularCoefficient");
   GLuint specularAlpha = glGetUniformLocation(shaderProgram, "specularAlpha");
   GLuint eyePosition = glGetUniformLocation(shaderProgram, "eye");

   GLuint color = glGetUniformLocation(colorOnlyShaderProgram, "inColor");

   while (!glfwWindowShouldClose(window)) {  // semmai la finestra dovesse chiudersi
      /* Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
       * In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
       */
      prevTime = currTime;
      currTime = glfwGetTime();

      pollInput(window);

      glClearColor(0.2, 0.2, 0.2, 1.0f);
      // Pulizia buffer colore e depth
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(colorOnlyShaderProgram);

      // Abilito il depth test per il check della profondità per la stampa a video degli oggetti
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      //glBindTexture(GL_TEXTURE_2D, texture1);
      // Attivazione canale texture (Texture Unit), per poter utilizzare il canale (che dentro è presente una texture)
      //glActiveTexture(GL_TEXTURE0);

      /* Scrivere nella location della variabile i valori del colore da assegnare al pixel;
       * Essendo macchina di stato, bisogna ricordare che la posizione influisce sull'azione delle chiamate
       * Quindi attenzione al posizionamento delle chiamate di modifica stato
       */

      SquareMatrix p(std::move(Projection::onAxisFOV2ClipProjectiveMatrix(camera)));
      SquareMatrix v(std::move(camera.world2ViewMatrix()));

      glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, p.getArray());
      glUniformMatrix4fv(viewMatrixUniform, 1, GL_TRUE, v.getArray());

      //glUniform3f(eyePosition, camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ());

      // Caricare vertexArrayObject interessato
      for (Model& model : objects) {
         model.setXRotation(degree2Radiants(90));
         SquareMatrix m(std::move(model.getWorldCoordinates()));
         glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, m.getArray());

         //light.setOrigin(light.getOrigin() + Float3(0.05f*sinf(glfwGetTime()), 0, 0));

         //glUniform3f(lightPosUniform, light.getOrigin().getX(), light.getOrigin().getY(), light.getOrigin().getZ());
         //glUniform3f(lightColorUniform, light.getColor().getRed(), light.getColor().getGreen(), light.getColor().getBlue());
         //TODO manage falloff
         //glUniform3f(lightIntensity, 1.0f, 1.0f, 1.0f);

         glUniform3f(ambientCoefficient, 0, 0, 0);
         glUniform3f(diffusiveCoefficient, 0.8, 0.8f, 0.8);
         glUniform3f(specularCoefficient, 0.5, 0.5, 0.5);
         glUniform1f(specularAlpha, 110);

         for (int j = 0; j < model.getMeshes().size(); ++j) {
            glBindVertexArray(vertexArrayObjects.at(j));
            // Chamata di disegno della primitiva
            glDrawElements(GL_TRIANGLES, model.getMeshes().at(j).getIndices().size(), GL_UNSIGNED_INT, 0);
         }
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
}

void cleanMemory() {
   // Liberazione della memoria
   for (int i = 0; i < vertexArrayObjects.size(); ++i) {
      glDeleteBuffers(1, &vertexBufferObjects.at(i));
      glDeleteBuffers(1, &elementBufferObjects.at(i));
      glDeleteVertexArrays(1, &vertexArrayObjects.at(i));
   }

   glDeleteProgram(colorOnlyShaderProgram);
}