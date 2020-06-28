#include "GL_Loader.h"

#include "Shaders.h"
#include "SceneObjects.h"
#include "../Animation/Scripts.h"
#include "Settings.h"

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <random>

void refreshWindowSize(GLFWwindow *window, int width, int height) {
   /* La Callback prevere azioni sull'immagine, per poi riproiettarla tramite glViewport
    * glViewport è la funzione per la trasformazione da NDC a Screen
    */
   X_RESOLUTION = width;
   Y_RESOLUTION = height;
   glfwGetWindowSize(window, &X_RESOLUTION, &Y_RESOLUTION);
   aspectRatio = static_cast<float>(Y_RESOLUTION) / static_cast<float>(X_RESOLUTION);
   camera.setAspectRatio(aspectRatio);

   glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X_RESOLUTION, Y_RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

   glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   glViewport(0, 0, width, height);
}

//---------------------------------------------------------------------------------//

void initializeGLFW() {
   // Inzializzazione di OpenGL per il render
   glfwInit();
   // Setup versione da utilizzare
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   /*
   // Attivazione multisampling
   // TODO manage this from program settings
   glfwWindowHint(GLFW_SAMPLES, 4);
    */

#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

bool setupWindowEnvironment() {
   // Creazione finestra, con nome, monitor da assegnare e finestre da cui dipendere
   window = glfwCreateWindow(X_RESOLUTION, Y_RESOLUTION, "Cotecchio Game", nullptr, nullptr);

   if (!window) {
      // Controllo in caso di errore di inizializzazione e pulizia del programma annessa
      std::cout << "Error INITIALISATION: window cannot be initialised." << std::endl;
      glfwTerminate();

      return false;
   }

   // Crea un contesto (link con il thread) di OpenGL alla finestra (permette l'impostazione della macchina di stato)
   glfwMakeContextCurrent(window);
   // Chiamare determinate callbacks per ogni azione (funzioni da richiamare in un certo evento)
   glfwSetWindowSizeCallback(window, refreshWindowSize);

   /*
   // Imposto input tramite mouse
   glfwSetCursorPosCallback(window, cursorPositionCallBack);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    */

   glfwSetScrollCallback(window, scrollCallBack);

   if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      // Controllo in caso di errore di caricamento puntatori alle funzioni della scheda video
      std::cout << "Error LOADING_GL: libraries cannot be called" << std::endl;
      glfwTerminate();

      return false;
   }

   glfwSetCursorPos(window, 0, 0);
   loadIcon("cotecchio.png");
   //loadIcon("cotecchio.png", "cotecchio_small.png");

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

   if (ENABLE_MULTISAMPLING) {
      // Attivazione multisampling (va creato un nuovo buffer che contenga le informazioni di colore con l'azione dell'algoritmo di MSAA)
      //IMPORTANTE: i sample per il Color Buffer DEVONO essere equivalenti a quelli del Depth e Color Buffer (per equivalenza di numero di pixel)
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, offlineTexture);

      // false per fixed sample location (da tenere false?) [con true, parte, immagino sia da implementare la posizione casuale dei campioni]
      // Si sceglie la funzione apposita per il Multisample
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTISAMPLING_LEVEL, GL_RGB, X_RESOLUTION, Y_RESOLUTION, GL_TRUE);

      glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, offlineTexture, 0);

      // Utilizzo di Renderbuffer (buffer per il rendering, solo per scrittura ma non come lettura; unicamente assegnabile)
      // Generazione del RenderBufferObject (l'uso del RBO è per la sua ottimizzazione interna a OpenGL nel conservare le informazioni colore
      glGenRenderbuffers(1, &offlineRenderBufferObject);
      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);

      // Preparazione del RBO (creiamo il depth e stencil per il render (poichè stiamo creando il framebuffer per il rendering effettivo della scena)
      // Si sceglie la funzione apposita per il Multisample
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_LEVEL, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);
      // Collego il RBO al Framebuffer creato, come buffer per Depth Test e Stencil Text
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offlineRenderBufferObject);

      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
   } else {
      glBindTexture(GL_TEXTURE_2D, offlineTexture);

      // nullptr solo per riservare memoria, non inizializzare ora la texture (sarà il fragment shader a riempire la texture)
      // Attenzione del tipo inserito (in questo caso è colore, quindi GL_RGB)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X_RESOLUTION, Y_RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

      // Nessuna distorsione: la texture sarà a tutto schermo visibile, inamovibile
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Necessità di collegare il color attachment (l'"indirizzo" della texture)
      // Stiamo collegando ora il colore(da definire il tipo di attach, ovvero GL_TEXTURE_2D
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offlineTexture, 0);

      // Utilizzo di Renderbuffer (buffer per il rendering, solo per scrittura ma non come lettura; unicamente assegnabile)
      // Generazione del RenderBufferObject (l'uso del RBO è per la sua ottimizzazione interna a OpenGL nel conservare le informazioni colore
      glGenRenderbuffers(1, &offlineRenderBufferObject);
      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);

      // Preparazione del RBO (creiamo il depth e stencil per il render (poichè stiamo creando il framebuffer per il rendering effettivo della scena)
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);
      // Collego il RBO al Framebuffer creato, come buffer per Depth Test e Stencil Text
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offlineRenderBufferObject);

      glBindTexture(GL_TEXTURE_2D, 0);
   }

   return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void compileShaders() {
   /* Creazione dello shader (vertex o fragment)
   * VERTEX SHADER
   * Restituisce GL unsigned int, indice/puntatore dell'oggetto vertex shader creato dalla GPU
   * Operazione su valori binari, invia chiamata sulla scheda grafica
   */

   compileShader("Graphics/Shader Files/phong_texture_vertex.glsl", "Graphics/Shader Files/phong_texture_fragment.glsl", phongShaderProgram);

   //--------------------------OFFLINE RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/offline_vertex.glsl", "Graphics/Shader Files/offline_fragment.glsl", offlineShaderProgram);

   //---------------------------CARD RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/cards_vertex.glsl", "Graphics/Shader Files/cards_fragment.glsl", cardsShader);

   //---------------------------STENCIL RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/color_vertex.glsl", "Graphics/Shader Files/color_fragment.glsl", colorShader);
}

void loadCards() {
   unsigned int values[40] {0};

   std::mt19937_64 gen(glfwGetTime());

   // TODO fix with correct cards from game rules
   int j = 10;
   for (unsigned int & value : values) {
      value = j++;
   }

   // Si mischiano così le carte in C++?
   std::shuffle(values, values+40, gen);
   std::shuffle(values, values+40, gen);
   std::shuffle(values, values+40, gen);

   for (auto& value : values) {
      cardsValue.push(value);
   }

   int playerID = 0;
   for (auto& player : players) {
      for (unsigned int i = 0; i < 40/sessionPlayers; ++i) {
         player.getCards().emplace_back(Card(cardsValue.top(), playerID));
         cardsValue.pop();
      }

      ++playerID;
   }
}

void loadObjects() {
   // TODO initial translation, rotation and scale
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
   textureUniforms.reserve(texturesLength);
   materialIndices.reserve(texturesLength);

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

   createPlayerPositions(3);
   loadCards();
   loadCardTextures();

   prepareScreenForOfflineRendering();

   prepareCardRendering();
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

void prepareCardRendering() {
   glGenVertexArrays(1, &cardVAO);
   glGenBuffers(1, &cardVBO);
   glGenBuffers(1, &cardVBO2);
   glGenBuffers(1, &cardVBO3);
   glGenBuffers(1, &cardEBO);

   glBindVertexArray(cardVAO);

   glBindBuffer(GL_ARRAY_BUFFER, cardVBO2);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, cardUVArray, GL_DYNAMIC_DRAW);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, cardVBO3);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, cardUVArray, GL_STATIC_DRAW);
   glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(3);

   glBindBuffer(GL_ARRAY_BUFFER, cardVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardEBO);

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, cardVertices, GL_DYNAMIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, vIndices, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
   glEnableVertexAttribArray(2);

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

void renderText() {

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

   blurUniform = glGetUniformLocation(offlineShaderProgram, "blurValue");

   GLuint cardModelMatrix = glGetUniformLocation(cardsShader, "model");
   GLuint cardViewMatrix = glGetUniformLocation(cardsShader, "view");
   GLuint cardProjectionMatrix = glGetUniformLocation(cardsShader, "projection");
   GLuint cardEyePosition = glGetUniformLocation(cardsShader, "eye");

   GLuint cardTexUnif = glGetUniformLocation(cardsShader, "cardTexture");
   GLuint backTexUnif = glGetUniformLocation(cardsShader, "backTexture");

   GLuint colorModelMatrix = glGetUniformLocation(colorShader, "model");
   GLuint colorViewMatrix = glGetUniformLocation(colorShader, "view");
   GLuint colorProjectionMatrix = glGetUniformLocation(colorShader, "projection");

   SquareMatrix p(4, {});
   SquareMatrix v(4, {});
   SquareMatrix m(4, {});

   SquareMatrix cM(4, {});


   while (!glfwWindowShouldClose(window)) {  // semmai la finestra dovesse chiudersi
      /* Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
       * In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
       */
      prevTime = currTime;
      currTime = glfwGetTime();

      light.setOrigin(light.getOrigin() + Float3(0, 0, 0.5f*sinf(glfwGetTime())));

      glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);

      pollInput(window);

      glClearColor(0.2, 0.2, 0.2, 1.0f);
      // Pulizia buffer colore e depth
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(phongShaderProgram);

      // Abilito back culling face (non si elaborano facce non visibili in base alla normale
      glEnable(GL_CULL_FACE);
      // Definisce la normale da calcolare in base all'ordine dei vertici (se come orari o antiorari)
      glCullFace(GL_BACK);
      //glFrontFace(GL_CW); // o CCW

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

      // Caricare vertexArrayObject interessato
      // TODO optimize by setting uniform locally in RAM
      glUniform1i(glGetUniformLocation(phongShaderProgram, "texture1"), 0);
      glUniform1i(glGetUniformLocation(phongShaderProgram, "bumpTexture"), 1);

      for (auto& object : objects) {
         m = std::move(object.getWorldCoordinates());
         glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, m.getArray());

         for (int j = 0; j < object.getMeshes().size(); ++j) {
            glUniform3f(ambientCoefficient, materials.at(materialIndices.at(j)).getAmbientCoeff().getX(),
                        materials.at(materialIndices.at(j)).getAmbientCoeff().getY(),
                        materials.at(materialIndices.at(j)).getAmbientCoeff().getZ());
            glUniform3f(diffusiveCoefficient, materials.at(materialIndices.at(j)).getDiffuseCoeff().getX(),
                        materials.at(materialIndices.at(j)).getDiffuseCoeff().getY(),
                        materials.at(materialIndices.at(j)).getDiffuseCoeff().getZ());
            glUniform3f(specularCoefficient, materials.at(materialIndices.at(j)).getSpecularCoeff().getX(),
                        materials.at(materialIndices.at(j)).getSpecularCoeff().getY(),
                        materials.at(materialIndices.at(j)).getSpecularCoeff().getZ());
            glUniform1f(specularAlpha, materials.at(materialIndices.at(j)).getShininess());

            /*
            if (materialIndices.at(j) == 1) {

            }
             */

            // Imposto lo uniform interessato con la texture unit in cui è presente la texture
            // Attivazione canale texture (Texture Unit), per poter utilizzare il canale (che dentro è presente una texture)
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

      glUseProgram(cardsShader);

      glDisable(GL_CULL_FACE);

      // Abilito Stencil test per l'outlining
      glEnable(GL_STENCIL_TEST);
      // Imposto le modalità di scrittura dello stencil test
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
      // Per ora viene disattivata la scrittura sul buffer
      glStencilMask(0x00);

      // Blend di trasparenza
      // TODO implement order-dependent transparency
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      glBindVertexArray(cardVAO);
      glBindBuffer(GL_ARRAY_BUFFER, cardVBO2);

      glUniform1i(cardTexUnif, 3);
      glUniform1i(backTexUnif, 4);

      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, cardTexture);

      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, backCardTexture);

      glUniform3f(cardEyePosition, camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ());
      glUniformMatrix4fv(cardProjectionMatrix, 1, GL_TRUE, p.getArray());
      glUniformMatrix4fv(cardViewMatrix, 1, GL_TRUE, v.getArray());

      glStencilFunc(GL_ALWAYS, 1, 0xFF);

      for (unsigned int pIndex = 0; pIndex < players.size(); ++pIndex) {
         if (pIndex == playerIndex) {
            glStencilMask(0xFF);
         }

         for (unsigned int i = 0; i < players.at(pIndex).getCards().size(); ++i) {
            cM = std::move(players.at(pIndex).getCards().at(i).getWorldCoordinates(i));
            players.at(pIndex).getCards().at(i).updateCoords();

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8, cardUVArray);

            glUniformMatrix4fv(cardModelMatrix, 1, GL_TRUE, cM.getArray());

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         }

         if (pIndex == playerIndex) {
            glStencilMask(0x00);
         }
      }

      // Genero outlining tramite Stencil test
      glUseProgram(colorShader);

      // Disabilito il Depth Test per poter aggiungere varie informazioni o effetti a schermo
      glDisable(GL_DEPTH_TEST);

      // Verrà stampato qualcosa su schermo solo se supera il controllo sottostante
      // Ovvero quando l'oggetto si trova fuori dalla maschera, ovvero il tratto per l'outlining

      // TODO fix here
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);

      glUniformMatrix4fv(colorProjectionMatrix, 1, GL_TRUE, p.getArray());
      glUniformMatrix4fv(colorViewMatrix, 1, GL_TRUE, v.getArray());

      for (auto & i : players.at(playerIndex).getCards()) {
         cM = std::move(i.getLocal2World() * Transform::scaleMatrix4(1.05, 1.05, 1.05));

         glUniformMatrix4fv(colorModelMatrix, 1, GL_TRUE, cM.getArray());

         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }

      // Da riattivare per permettere le modifiche al buffer
      glEnable(GL_DEPTH_TEST);
      glStencilMask(0xFF);

      /*
      double x, y;
      glfwGetCursorPos(window, &x, &y);
       */

      for (auto& player : players) {
         for (unsigned int i = 0; i < player.getCards().size(); ++i) {
            cM = std::move(player.getCards().at(i).getWorldCoordinates(i));

            glUniformMatrix4fv(cardModelMatrix, 1, GL_TRUE, cM.getArray());

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         }
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      // Al cambio di framebuffer, è necessario reimpostarlo, come un normale ciclo
      glUseProgram(offlineShaderProgram);

      glClearColor(1, 1, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      // Accediamo alla texture che adesso è il nostro colore, e la assegneremo a schermo

      if (MENU_TRANSLATION_CAMERA) {
         glUniform1f(blurUniform, blurValue);
      }

      glBindVertexArray(sVAO);
      glUniform1i(glGetUniformLocation(offlineShaderProgram, "offlineRendering"), 2);
      glActiveTexture(GL_TEXTURE2);
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
   // TODO complete adding remaining variables
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