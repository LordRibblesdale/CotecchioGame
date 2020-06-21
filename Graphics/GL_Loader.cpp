#include "GL_Loader.h"

#include "Shaders.h"
#include "SceneObjects.h"

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#endif


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
      // TODO implement methods with reference access (reduce copies)
      tmp = std::move(Rotation::axisZRotateVertex3(tmp, -angle));

      camera.setYawAngle(acosf(tmp.getNormalized().getX()));

      tmp += camera.getEye();
      camera.setLookAt(tmp);

      prevXPos = xPos;
   }

   if (diffY != 0) {
      float angle = atanf(diffY/100.0f);

      Float3 tmp(std::move((camera.getLookAt() - camera.getEye()).getNormalized()));

      //TODO fix angle rotation
      tmp = std::move(Rotation::axisZRotateVertex3(tmp, camera.getYawAngle()*0.5f));
      tmp = std::move(Rotation::axisXRotateVertex3(tmp, angle));
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
      vec.setZ(0);
      camera.setEye(camera.getEye() - speed*vec);
      camera.setLookAt(camera.getLookAt() - speed*vec);
   }

   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      float tmp = vec.getX();
      vec.setX(-vec.getY());
      vec.setY(tmp);
      vec.setZ(0);
      camera.setEye(camera.getEye() + speed*vec);
      camera.setLookAt(camera.getLookAt() + speed*vec);
   }

   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      camera.setEye(camera.getEye() + speed*vec);
      camera.setLookAt(camera.getLookAt() + speed*vec);
   }

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      Float3 vec(camera.getEye() - camera.getLookAt());
      vec.normalize();
      camera.setEye(camera.getEye() - speed*vec);
      camera.setLookAt(camera.getLookAt() - speed*vec);
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
         camera.setEye(cameraRotation->rotateCamera(camera.getEye(), diff));

         sumTime += diff;
      } else {
         sumTime = 0;
         TRANSFORM_CAMERA = false;
      }
   }
}

//---------------------------------------------------------------------------------//

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

bool setupWindowEnvironment() {
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
   glfwSetCursorPosCallback(window, cursorPositionCallBack);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glfwSetScrollCallback(window, scrollCallBack);

   if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      // Controllo in caso di errore di caricamento puntatori alle funzioni della scheda video
      std::cout << "Error LOADING_GL: libraries cannot be called";
      glfwTerminate();

      return false;
   }

   //glGenFramebuffers(GL_FRAMEBUFFER, &offlineFrameBuffer);

   return true;
}


bool setupOfflineRendering() {
   // Generazione del framebuffer
   glGenFramebuffers(1, &offlineFrameBuffer);

   /* Collego il bind col framebuffer che ho creato, per assegnargli tutto il necessario
    * -> E' necessario almeno un buffer e almeno un color attachment
    *    (Attachment, collegamento al buffer)
    * -> Non verranno però inizializzati ma invece solo riservati in memoria
    */
   glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);

   // Controllare se il framebuffer è stato creato correttamente
   /* Imposta la viewport con una texture, cosicchè al momento del rendering, il risultato venga applicato
   *  all'offline rendering e visualizzato a schermo
   * La texture permetterà l'output su schermo, mentre il renderobject provvede a conservare le informazioni
   */
   glGenTextures(1, &offlineTexture);
   glBindTexture(GL_TEXTURE_2D, offlineTexture);

   // nullptr solo per riservare memoria, non inizializzare ora la texture (sarà il fragment shader a riempire la texture)
   // Attenzione del tipo inserito (in questo caso è colore, quindi GL_RGB)
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

   // Nessuna distorsione: la texture sarà a tutto schermo visibile, inamovibile
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Necessità di collegare il color attachment (l'"indirizzo" della texture)
   // Stiamo collegando ora il colore(da definire il tipo di attach, ovvero GL_TEXTURE_2D
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offlineTexture, 0);
   // Possibile utilizzo di Renderbuffer (buffer per il rendering, solo per scrittura ma non come lettura; unicamente assegnabile)

   // Generazione del RenderBufferObject (l'uso del RBO è per la sua ottimizzazione interna a OpenGL nel conservare le informazioni colore
   glGenRenderbuffers(1, &offlineRenderBufferObject);
   glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);

   // Preparazione del RBO (creiamo il depth e stencil per il render (poichè stiamo creando il framebuffer per il rendering effettivo della scena)
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

   return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
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
   loadShader(source, "Graphics/Shader Files/phong_texture_vertex.glsl");
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

   loadShader(source, "Graphics/Shader Files/phong_texture_fragment.glsl");
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
   phongShaderProgram = glCreateProgram();

   // Aggiunta del programma dei vari shader
   glAttachShader(phongShaderProgram, vertexShader);
   glAttachShader(phongShaderProgram, fragmentShader);

   // Link del programma, unisce le unità programmabili (shader)
   glLinkProgram(phongShaderProgram);

   // Controllo del link programma, status
   glGetProgramiv(phongShaderProgram, GL_LINK_STATUS, &report);

   if (!report) {
      glGetProgramInfoLog(phongShaderProgram, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_LINK_PROGRAM: " << infoLog << std::endl;
   }

   // Pulizia memoria dopo la compilazione e link
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   //--------------------------OFFLINE RENDERING--------------------------------//

   vertexShader = glCreateShader(GL_VERTEX_SHADER);

   loadShader(source, "Graphics/Shader Files/offline_vertex.glsl");
   charSource = const_cast<char *>(source.c_str());

   if (!charSource) {
      std::cout << "Error VERTEX_FILE_IMPORT" << std::endl;
   }

   glShaderSource(vertexShader, 1, &charSource, nullptr);
   glCompileShader(vertexShader);

   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_VERTEX" << std::endl;
   }


   loadShader(source, "Graphics/Shader Files/offline_fragment.glsl");
   charSource = const_cast<char *>(source.c_str());

   if (!charSource) {
      std::cout << "Error FRAGMENT_FILE_IMPORT" << std::endl;
   }

   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &charSource, nullptr);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_FRAGMENT: " << infoLog << std::endl;
   }

   offlineShaderProgram = glCreateProgram();

   glAttachShader(offlineShaderProgram, vertexShader);
   glAttachShader(offlineShaderProgram, fragmentShader);

   glLinkProgram(offlineShaderProgram);

   glGetProgramiv(offlineShaderProgram, GL_LINK_STATUS, &report);

   if (!report) {
      glGetProgramInfoLog(offlineShaderProgram, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_LINK_PROGRAM: " << infoLog << std::endl;
   }

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

void loadObjects() {
   // TODO setup diffusive & specular

   const char* TABLE_NAME = "Table";
   std::string s(TABLE_ASSETS_LOCATION + TABLE_NAME + ".obj");
   Assimp::Importer importer;
   const aiScene* scene(importer.ReadFile(s, aiProcess_Triangulate));

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "Error ASSIMP_SCENE_LOADING: scene not loaded." << std::endl << importer.GetErrorString() << std::endl;
   } else {
      Model table(TABLE_ASSETS_LOCATION, TABLE_NAME);
      table.processNode(scene->mRootNode, scene);

      table.setXRotation(degree2Radiants(90));
      objects.emplace_back(table);
   }

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

   for (const auto& object : objects) {
      for (const auto& mesh : object.getMeshes()) {
         generateObjects(mesh);
      }

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
      loadTexture(object.getLocation(), object.getName());
   }

   prepareScreenForOfflineRendering();
}

void prepareScreenForOfflineRendering() {
   glGenVertexArrays(1, &sVAO);
   glGenBuffers(1, &sVBO);
   glGenBuffers(1, &sEBO);
   glBindVertexArray(sVAO);

   glBindBuffer(GL_ARRAY_BUFFER, sVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 20, screen, GL_DYNAMIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, screenIndices, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   // Imposta il nuovo buffer a 0, ovvero slega il bind dall'array (per evitare di sovrascrivere)
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // Unbind del VAO precedentemente assegnato per evitare sovrascritture non volute
   glBindVertexArray(0);

   // Da de-bindare dopo poichè VAO contiene i vari bind dell'EBO, se si de-bindasse prima, il VAO non avrebbe l'EBO
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
   unsigned int size = 3*mesh.getVertices().size() + 2*mesh.getTextureUnwrap().size() + 3*mesh.getNormals().size();
   GLfloat attributes[size];

   int index = 0;
   for (int i = 0; i < mesh.getVertices().size(); ++i) {
      attributes[index++] = mesh.getVertices().at(i).getX();
      attributes[index++] = mesh.getVertices().at(i).getY();
      attributes[index++] = mesh.getVertices().at(i).getZ();

      attributes[index++] = mesh.getTextureUnwrap().at(i).getX();
      attributes[index++] = mesh.getTextureUnwrap().at(i).getY();

      attributes[index++] = mesh.getNormals().at(i).getX();
      attributes[index++] = mesh.getNormals().at(i).getY();
      attributes[index++] = mesh.getNormals().at(i).getZ();
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
    *
    * Il valore 3 dice quanti vertici
    * Lettura del buffer, con un offset di lettura dei 3 valori GL_FLOAT di 3 posizioni;
    * Abilita gli attributi passatigli
    *
    */
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (5*sizeof(GLfloat)));
   glEnableVertexAttribArray(2);

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
   GLuint projectionMatrixUniform = glGetUniformLocation(phongShaderProgram, "projection");
   GLuint viewMatrixUniform = glGetUniformLocation(phongShaderProgram, "view");
   GLuint modelMatrixUniform = glGetUniformLocation(phongShaderProgram, "model");

   GLuint lightPosUniform = glGetUniformLocation(phongShaderProgram, "lightPos");
   GLuint lightColorUniform = glGetUniformLocation(phongShaderProgram, "lightColor");
   GLuint lightIntensity = glGetUniformLocation(phongShaderProgram, "lightIntensity");

   GLuint ambientCoefficient = glGetUniformLocation(phongShaderProgram, "ambientCoefficient");
   GLuint diffusiveCoefficient = glGetUniformLocation(phongShaderProgram, "diffusiveCoefficient");
   GLuint specularCoefficient = glGetUniformLocation(phongShaderProgram, "specularCoefficient");
   GLuint specularAlpha = glGetUniformLocation(phongShaderProgram, "specularAlpha");
   GLuint eyePosition = glGetUniformLocation(phongShaderProgram, "eye");

   SquareMatrix p(4, {});
   SquareMatrix v(4, {});
   SquareMatrix m(4, {});

   while (!glfwWindowShouldClose(window)) {  // semmai la finestra dovesse chiudersi
      /* Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
       * In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
       */
      //prevTime = currTime;
      //currTime = glfwGetTime();

      glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);

      pollInput(window);

      glClearColor(0.2, 0.2, 0.2, 1.0f);
      // Pulizia buffer colore e depth
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(phongShaderProgram);

      // Abilito il depth test per il check della profondità per la stampa a video degli oggetti
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      /* Scrivere nella location della variabile i valori del colore da assegnare al pixel;
       * Essendo macchina di stato, bisogna ricordare che la posizione influisce sull'azione delle chiamate
       * Quindi attenzione al posizionamento delle chiamate di modifica stato
       */

      p = std::move(Projection::onAxisFOV2ClipProjectiveMatrix(camera));
      v = std::move(camera.world2ViewMatrix());

      glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, p.getArray());
      glUniformMatrix4fv(viewMatrixUniform, 1, GL_TRUE, v.getArray());

      glUniform3f(eyePosition, camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ());

      glUniform3f(lightPosUniform, light.getOrigin().getX(), light.getOrigin().getY(), light.getOrigin().getZ());
      glUniform3f(lightColorUniform, light.getColor().getRed(), light.getColor().getGreen(), light.getColor().getBlue());
      //TODO manage falloff
      glUniform3f(lightIntensity, 1.0f, 1.0f, 1.0f);

      glUniform3f(ambientCoefficient, 0, 0, 0);
      glUniform3f(diffusiveCoefficient, 0.8, 0.8f, 0.8f);
      glUniform3f(specularCoefficient, 0.1f, 0.1f, 0.1f);
      glUniform1f(specularAlpha, 110);

      // Caricare vertexArrayObject interessato

      glUniform1i(glGetUniformLocation(phongShaderProgram, "texture1"), 0);
      glUniform1i(glGetUniformLocation(phongShaderProgram, "bumpTexture"), 1);

      for (auto& object : objects) {
         m = std::move(object.getWorldCoordinates());
         glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, m.getArray());

         for (int j = 0; j < object.getMeshes().size(); ++j) {
            //glBindTexture(GL_TEXTURE_2D, texture1);
            // Attivazione canale texture (Texture Unit), per poter utilizzare il canale (che dentro è presente una texture)
            // glActiveTexture(GL_TEXTURE0);

            // Imposto lo uniform interessato con la texture unit in cui è presente la texture
            glActiveTexture(GL_TEXTURE0);
            // Il bind sulla variabile texture1 ora si riverisce alla texture unit a cui è stata collegata
            glBindTexture(GL_TEXTURE_2D, textureUniforms.at(j));

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, bumpUniforms.at(j));

            glBindVertexArray(vertexArrayObjects.at(j));
            // Chamata di disegno della primitiva
            glDrawElements(GL_TRIANGLES, object.getMeshes().at(j).getIndices().size(), GL_UNSIGNED_INT, 0);
         }
      }

      // Disabilito il Depth Test per poter aggiungere varie informazioni o effetti a schermo
      glDisable(GL_DEPTH_TEST);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      // Al cambio di framebuffer, è necessario reimpostarlo, come un normale ciclo
      glClearColor(1, 1, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      // Accediamo alla texture che adesso è il nostro colore, e la assegneremo a schermo
      glUseProgram(offlineShaderProgram);
      glBindVertexArray(sVAO);
      glBindTexture(GL_TEXTURE_2D, offlineTexture);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

   glDeleteBuffers(1, &sEBO);
   glDeleteBuffers(1, &sVBO);
   glDeleteVertexArrays(1, &sVAO);

   glDeleteProgram(phongShaderProgram);
   glDeleteFramebuffers(1, &offlineFrameBuffer);
}