#include "GL_Loader.h"

#include "Shaders.h"
#include "SceneObjects.h"
#include "../Scripting/Scripts.h"
#include "Settings.h"
#include "../Model/Triangle.h"
#include "../Utilities/Ray.h"

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#endif

//#include <ft2build.h>
//#include FT_FREETYPE_H

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

   if (ENABLE_MULTISAMPLING) {
      glBindFramebuffer(GL_FRAMEBUFFER, secondaryFrameBuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_LEVEL, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);

      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaOfflineTexture);
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTISAMPLING_LEVEL, GL_RGB, X_RESOLUTION, Y_RESOLUTION, GL_TRUE);
   } else {
      glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);
      glBindTexture(GL_TEXTURE_2D, offlineTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X_RESOLUTION, Y_RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);
   }

   glBindTexture(GL_TEXTURE_2D, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   glViewport(0, 0, width, height);
}

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
   glfwSetScrollCallback(window, scrollCallBack);

   if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      // Controllo in caso di errore di caricamento puntatori alle funzioni della scheda video
      std::cout << "Error LOADING_GL: libraries cannot be called" << std::endl;
      glfwTerminate();

      return false;
   }

   loadIcon("cotecchio.png");
   //loadIcon("cotecchio.png", "cotecchio_small.png");

   return true;
}


bool setupOfflineRendering() {
   bool status = true;

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
      glGenTextures(1, &msaaOfflineTexture);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaOfflineTexture);

      // false per fixed sample location (da tenere false?) [con true, parte, immagino sia da implementare la posizione casuale dei campioni]
      // Si sceglie la funzione apposita per il Multisample
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTISAMPLING_LEVEL, GL_RGB, X_RESOLUTION, Y_RESOLUTION, GL_TRUE);

      glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaOfflineTexture, 0);

      // Utilizzo di Renderbuffer (buffer per il rendering, solo per scrittura ma non come lettura; unicamente assegnabile)
      // Generazione del RenderBufferObject (l'uso del RBO è per la sua ottimizzazione interna a OpenGL nel conservare le informazioni colore
      glGenRenderbuffers(1, &offlineRenderBufferObject);
      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);

      // Preparazione del RBO (creiamo il depth e stencil per il render (poichè stiamo creando il framebuffer per il rendering effettivo della scena)
      // Si sceglie la funzione apposita per il Multisample
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_LEVEL, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);
      // Collego il RBO al Framebuffer creato, come buffer per Depth Test e Stencil Text
      glBindRenderbuffer(GL_RENDERBUFFER, 0);

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offlineRenderBufferObject);

      status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      // Nuovo framebuffer per applicare post processing al risultato elaborato da antialiasing
      glGenFramebuffers(1, &secondaryFrameBuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, secondaryFrameBuffer);
   } else {
      // Utilizzo di Renderbuffer (buffer per il rendering, solo per scrittura ma non come lettura; unicamente assegnabile)
      // Generazione del RenderBufferObject (l'uso del RBO è per la sua ottimizzazione interna a OpenGL nel conservare le informazioni colore
      glGenRenderbuffers(1, &offlineRenderBufferObject);
      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);

      // Preparazione del RBO (creiamo il depth e stencil per il render (poichè stiamo creando il framebuffer per il rendering effettivo della scena)
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);
      // Collego il RBO al Framebuffer creato, come buffer per Depth Test e Stencil Text
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offlineRenderBufferObject);
   }

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

   status = status && glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   return status;
}

void compileShaders() {
   /* Creazione dello shader (vertex o fragment)
   * VERTEX SHADER
   * Restituisce GL unsigned int, indice/puntatore dell'oggetto vertex shader creato dalla GPU
   * Operazione su valori binari, invia chiamata sulla scheda grafica
   */

   compileShader("Graphics/Shader Files/texture_vertex.glsl", "Graphics/Shader Files/texture_fragment.glsl", shaderProgram);

   //--------------------------OFFLINE RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/offline_vertex.glsl", "Graphics/Shader Files/offline_fragment.glsl", offlineShaderProgram);

   //---------------------------CARD RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/cards_vertex.glsl", "Graphics/Shader Files/cards_fragment.glsl", cardsShader);

   //---------------------------STENCIL RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/color_vertex.glsl", "Graphics/Shader Files/color_fragment.glsl", colorShader);

   //----------------------------SHADOW RENDERING--------------------------------//

   compileShader("Graphics/Shader Files/light_vertex.glsl", "Graphics/Shader Files/light_fragment.glsl", lightShader);
}

void loadCards() {
   unsigned int values[40] {0};

    std::mt19937_64 gen(glfwGetTime());

   // TODO fix with correct cards from game rules
   int j = 10;
   for (unsigned int & value : values) {
      value = j++;
   }

   std::shuffle(values, values+40, gen);
   gen.seed(glfwGetTime());
   std::shuffle(values, values+40, gen);
   gen.seed(glfwGetTime());
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

void loadObject(const std::string& location, const std::string& objName, const unsigned int& objType) {
   std::string s(location + objName + ".obj");
   const aiScene* scene(importer->ReadFile(s, aiProcess_Triangulate));

   if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "Error ASSIMP_SCENE_LOADING: scene not loaded." << std::endl << importer->GetErrorString() << std::endl;
   } else {
      Model table(location, objName);
      table.processNode(scene->mRootNode, scene);

      if (objType == TABLE_TYPE) {
         table.setTexturesEnabled(true);
         table.setXRotation(degree2Radiants(90));
      } else if (objType == WORLD_TYPE) {
         table.setTexturesEnabled(false);
         table.setNeedsNoCulling(true);
         table.setXTranslation(1);
         table.setXRotation(degree2Radiants(90));
      }

      objects.emplace_back(std::move(table));
   }

   importer->FreeScene();
}

void loadObjects() {
   // TODO initial translation, rotation and scale
   importer = std::move(std::make_unique<Assimp::Importer>());

   loadObject(DATA_ASSETS_LOCATION, "World", WORLD_TYPE);

   loadObject(TABLE_ASSETS_LOCATION, "Table", TABLE_TYPE);

   unsigned long verticesLenght = 0;
   unsigned long texturesLength = 0;

   for (const Model& model : objects) {
      for (const Mesh& mesh : model.getMeshes()) {
         verticesLenght += mesh.getVertices().size();
      }

      if (model.doesHaveTextures()) {
         texturesLength += model.getMeshes().size();
      }
   }

   vertexArrayObjects.reserve(verticesLenght);
   vertexBufferObjects.reserve(verticesLenght);
   textureUniforms.reserve(texturesLength);

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

      loadTexture(object.getLocation(), object.getName(), object.doesHaveTextures());
   }

   createPlayerPositions(3);
   loadCards();
   loadCardTextures();

   prepareScreenForOfflineRendering();

   prepareCardRendering();

   prepareSceneLights();
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

void setupLightMap(Light* light) {
   // Genero il buffer per il calcolo della profondità (confronto tra profondità)
   glGenTextures(1, &light->getDepthMapAsReference());
   glBindTexture(GL_TEXTURE_2D, light->getDepthMapAsReference());
   glActiveTexture(GL_TEXTURE10);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_QUALITY, SHADOW_QUALITY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); // Ancora da non assegnare
   // Da analizzare come classica texture per l'accesso valori
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // Clamp to edge per estendere il risultato fuori dai limiti della mappa (non copre tutta la scena)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   // Genero il framebuffer per il render della shadowmap
   glGenFramebuffers(1, &light->getFrameBufferAsReference());
   glBindFramebuffer(GL_FRAMEBUFFER, light->getFrameBufferAsReference());
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light->getDepthMapAsReference(), 0);
   // Disattivo la scrittura e lettura, verrà usato solo per la profondità
   glReadBuffer(GL_NONE);
   glDrawBuffer(GL_NONE);

   std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void prepareSceneLights() {
   lights.reserve(1);
   lights.emplace_back(new SpotLight(std::move(Float3(0, 0, 15)), Float3(0, 0, 0), Color(1, 1, 1), 10, degree2Radiants(40), degree2Radiants(60)));

   setupLightMap(lights.at(0).get());
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

   // GL_STATIC_DRAW??
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, attributes, GL_DYNAMIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getIndices().size(), mesh.getIndices().data(), GL_STATIC_DRAW);

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

void renderCardsInLoop(SquareMatrix& cardModelM, const GLuint& cardModelMatrix, SquareMatrix& viewM_V2W, unsigned int& pIndex,
        size_t& i, bool& hasSelectedCard, unsigned int& selectedIndex, int x, int y) {
   //std::vector<SquareMatrix> matrices;
   //matrices.reserve(4);

   //players.at(pIndex).getCards().at(i).getWorldCoordinates(i, matrices);

   cardModelM = std::move(players.at(pIndex).getCards().at(i).getWorldCoordinates(i));

   if (pIndex == playerIndex) {
      if (!hasSelectedCard) {
         if ((x >= 0 && x <= X_RESOLUTION) && (y >= 0 && y <= Y_RESOLUTION)) {
            /* Increase performance from ([3*n^3 + 4*n^2] to [12*n^2])
               n = 4 -> 256 to 192 operations

            Float4 p0(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                            Float4(cardVertices[0], cardVertices[1], cardVertices[2], 1))))));
            Float4 p1(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                            Float4(cardVertices[6], cardVertices[7], cardVertices[8], 1))))));
            Float4 p2(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                    Float4(cardVertices[12], cardVertices[13], cardVertices[14], 1))))));
            Float4 p3(matrices.at(0).multiplyVector(
                    matrices.at(1).multiplyVector(
                            matrices.at(2).multiplyVector(
                                    matrices.at(3).multiplyVector(
                                    Float4(cardVertices[18], cardVertices[19], cardVertices[20], 1))))));
                                    */

            Float4 p0(cardModelM.multiplyVector(Float4(cardVertices[0], cardVertices[1], cardVertices[2], 1)));
            Float4 p1(cardModelM.multiplyVector(Float4(cardVertices[6], cardVertices[7], cardVertices[8], 1)));
            Float4 p2(cardModelM.multiplyVector(Float4(cardVertices[12], cardVertices[13], cardVertices[14], 1)));
            Float4 p3(cardModelM.multiplyVector(Float4(cardVertices[18], cardVertices[19], cardVertices[20], 1)));

            Triangle cardT1(p0, p1, p2);
            Triangle cardT2(p2, p1, p3);

            // NDC -> Clip Space
            float xProj = (((2.0f*x)/static_cast<float>(X_RESOLUTION)) -1);
            float yProj = -(((2.0f*y)/static_cast<float>(Y_RESOLUTION)) -1);  // MINUS per via dell'asse invertito Y SCREEN rispetto a Y CLIP
            float zProj = -camera.getNear();

            // NDC Space -> View Space
            // ON-AXIS CAMERA
            Float4 clipPoint(xProj * camera.getRight(), yProj * camera.getTop(), zProj, 1);

            // View Space -> World Space
            clipPoint = std::move(viewM_V2W.multiplyVector(clipPoint));

            Ray ray(camera.getEye(), (Float3(clipPoint.getFloat3()) - camera.getEye()).getNormalized());

            // Implement multithreading w/ sync for better performance?
            TriangleIntersection i1(ray.getTriangleIntersection(cardT1));
            TriangleIntersection i2(ray.getTriangleIntersection(cardT2));

            if (i1.getHasIntersected() || i2.getHasIntersected()) {
               players.at(pIndex).getCards().at(i).setIsSelected(true);

               hasSelectedCard = true;
               selectedIndex = i;

               glStencilMask(0xFF);
            }
         }
      }
   }

   if (selectedIndex != i) {
      players.at(pIndex).getCards().at(i).updateCoords();

      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8, cardUVArray);

      glUniformMatrix4fv(cardModelMatrix, 1, GL_TRUE, cardModelM.getArray());

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }

}

void render() {
   // Collezione indici per inviare dati allo shader
   GLuint projectionMatrixUniform = glGetUniformLocation(shaderProgram, "projection");
   GLuint viewMatrixUniform = glGetUniformLocation(shaderProgram, "view");
   GLuint modelMatrixUniform = glGetUniformLocation(shaderProgram, "model");

   GLuint texUnif = glGetUniformLocation(shaderProgram, "texture1");
   GLuint bumpUnif = glGetUniformLocation(shaderProgram, "bumpTexture");

   GLuint lightPosUniform = glGetUniformLocation(shaderProgram, "lightPos");
   GLuint lightColorUniform = glGetUniformLocation(shaderProgram, "lightColor");
   GLuint lightIntensity = glGetUniformLocation(shaderProgram, "lightIntensity");

   GLuint ambientCoefficient = glGetUniformLocation(shaderProgram, "ambientCoefficient");
   GLuint diffusiveCoefficient = glGetUniformLocation(shaderProgram, "diffusiveCoefficient");
   GLuint specularCoefficient = glGetUniformLocation(shaderProgram, "specularCoefficient");
   GLuint specularAlpha = glGetUniformLocation(shaderProgram, "specularAlpha");
   GLuint eyePosition = glGetUniformLocation(shaderProgram, "eye");

   GLuint gammaUniform = glGetUniformLocation(shaderProgram, "gammaCorrection");

   GLuint fixedColorUniform = glGetUniformLocation(shaderProgram, "color");

   GLuint blurUniform = glGetUniformLocation(offlineShaderProgram, "blurValue");

   GLuint cardModelMatrix = glGetUniformLocation(cardsShader, "model");
   GLuint cardViewMatrix = glGetUniformLocation(cardsShader, "view");
   GLuint cardProjectionMatrix = glGetUniformLocation(cardsShader, "projection");
   GLuint cardEyePosition = glGetUniformLocation(cardsShader, "eye");
   GLuint playerIndexUniform = glGetUniformLocation(cardsShader, "currentPlayer");
   GLuint viewPlayerUniform = glGetUniformLocation(cardsShader, "viewPlayer");

   GLuint cardTexUnif = glGetUniformLocation(cardsShader, "cardTexture");
   GLuint backTexUnif = glGetUniformLocation(cardsShader, "backTexture");

   GLuint colorModelMatrix = glGetUniformLocation(colorShader, "model");
   GLuint colorViewMatrix = glGetUniformLocation(colorShader, "view");
   GLuint colorProjectionMatrix = glGetUniformLocation(colorShader, "projection");

   GLuint lightSpaceMatrixUniform = glGetUniformLocation(lightShader, "lightSpaceMatrix");
   GLuint modelLightShaderUniform = glGetUniformLocation(lightShader, "model");
   GLuint lsmMainShaderUniform = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
   GLuint depthMapUniform = glGetUniformLocation(shaderProgram, "depthMap");

   SquareMatrix projM_V2C(4, {});
   SquareMatrix viewM_W2V(4, {});
   SquareMatrix viewM_V2W(4, {});
   //SquareMatrix modelM_L2W(4, {});

   SquareMatrix cardModelM(4, {});

   std::vector<SquareMatrix> lightSpaceMs;
   lightSpaceMs.reserve(lights.size());

   for (auto i = 0; i < lights.size(); ++i) {
      lightSpaceMs.emplace_back(SquareMatrix(4, {}));
   }

   unsigned int skipVertexIndex = 0;
   unsigned int skipTextureIndex = 0;
   unsigned int skipBumpIndex = 0;

   unsigned int flipCardRenderingIndex = ceil(players.size()/2.0f);

   // Abilito Stencil test per l'outlining
   glEnable(GL_STENCIL_TEST);

   // Imposto le modalità di scrittura dello stencil test
   glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

   while (!glfwWindowShouldClose(window)) {  // semmai la finestra dovesse chiudersi
      /* Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
       * In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
       */
      prevTime = currTime;
      currTime = glfwGetTime();

      pollInput(window);

      lights.at(0).get()->setOrigin(lights.at(0).get()->getOrigin() + Float3(0, 0, 0.05f*sinf(glfwGetTime())));

      // Rendering shadow map
      glUseProgram(lightShader);

      // Imposto la viewport per il render della shadow map
      glViewport(0, 0, SHADOW_QUALITY, SHADOW_QUALITY);

      /*
      for (unsigned int i = 0; i < lights.size(); ++i) {
         glBindFramebuffer(GL_FRAMEBUFFER, lights.at(i)->getFrameBufferAsReference());
         glClear(GL_DEPTH_BUFFER_BIT);

         if (dynamic_cast<SpotLight*>(lights.at(i).get())) {
            lightSpaceMs.at(i) = std::move(Projection::onAxisFOV2ClipProjectiveMatrix(*lights.at(i)->getCamera()) *
                                                      lights.at(i)->getCamera()->world2ViewMatrix());
         } else {
            // TODO complete
         }

         glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_TRUE, lightSpaceMs.at(i).getArray());

         for (auto& object : objects) {
            //modelM_L2W = object.getWorldCoordinates();
            glUniformMatrix4fv(modelLightShaderUniform, 1, GL_TRUE, object.getWorldCoordinates().getArray());

            for (int j = 0; j < object.getMeshes().size(); ++j) {
               glBindVertexArray(vertexArrayObjects.at(j));
               // Chamata di disegno della primitiva
               glDrawElements(GL_TRIANGLES, object.getMeshes().at(j).getIndices().size(), GL_UNSIGNED_INT, 0);
            }
         }
      }
       */

      skipVertexIndex = 0;

      glBindFramebuffer(GL_FRAMEBUFFER, lights.at(0)->getFrameBufferAsReference());
      glClear(GL_DEPTH_BUFFER_BIT);
      lightSpaceMs.at(0) = std::move(Projection::onAxisFOV2ClipProjectiveMatrix(*lights.at(0)->getCamera()) *
                                     lights.at(0)->getCamera()->world2ViewMatrix());

      glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_TRUE, lightSpaceMs.at(0).getArray());

      for (auto& object : objects) {
         //modelM_L2W = object.getWorldCoordinates();
         glUniformMatrix4fv(modelLightShaderUniform, 1, GL_TRUE, object.getWorldCoordinates().getArray());

         for (const auto& mesh : object.getMeshes()) {
            glBindVertexArray(vertexArrayObjects.at(skipVertexIndex++));
            // Chamata di disegno della primitiva
            glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
         }
      }

      glBindFramebuffer(GL_FRAMEBUFFER, offlineFrameBuffer);
      glViewport(0, 0, X_RESOLUTION, Y_RESOLUTION);

      glClearColor(0.2, 0.2, 0.2, 1.0f);
      // Pulizia buffer colore, depth e stencil
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Esempio: appena modificato, agisce in base alle modifiche effettuate (stato del sistema)

      // Imposta tutte le chiamate tramite shaderProgram, iniziando la pipeline
      glUseProgram(shaderProgram);

      // Abilito back culling face (non si elaborano facce non visibili in base alla normale
      glEnable(GL_CULL_FACE);
      // Definisce la normale da calcolare in base all'ordine dei vertici (se come orari o antiorari)
      glCullFace(GL_BACK);
      //glFrontFace(GL_CW); // o CCW

      // Abilito il depth test per il check della profondità per la stampa a video degli oggetti
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      // Per ora viene disattivata la scrittura sul buffer
      glStencilMask(0x00);

      /* Scrivere nella location della variabile i valori del colore da assegnare al pixel;
       * Essendo macchina di stato, bisogna ricordare che la posizione influisce sull'azione delle chiamate
       * Quindi attenzione al posizionamento delle chiamate di modifica stato
       */

      projM_V2C = std::move(Projection::onAxisFOV2ClipProjectiveMatrix(camera));
      viewM_V2W = std::move(camera.view2WorldMatrix());
      viewM_W2V = std::move(SquareMatrix::calculateInverse(viewM_V2W));

      glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, projM_V2C.getArray());
      glUniformMatrix4fv(viewMatrixUniform, 1, GL_TRUE, viewM_W2V.getArray());
      // TODO fix for multiple lights
      glUniformMatrix4fv(lsmMainShaderUniform, 1, GL_TRUE, lightSpaceMs.at(0).getArray());

      glUniform3f(eyePosition, camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ());

      // TODO set multiple lights
      glUniform3f(lightPosUniform, lights.at(0).get()->getOrigin().getX(), lights.at(0).get()->getOrigin().getY(), lights.at(0).get()->getOrigin().getZ());
      glUniform3f(lightColorUniform, lights.at(0).get()->getColor().getRed(), lights.at(0).get()->getColor().getGreen(), lights.at(0).get()->getColor().getBlue());
      //TODO manage falloff (on CPU or GPU?)
      glUniform1f(lightIntensity, lights.at(0).get()->getIntensity());

      // Caricare vertexArrayObject interessato
      glUniform1i(texUnif, 0);
      glUniform1i(bumpUnif, 1);
      // TODO set multiple lights
      glUniform1i(depthMapUniform, 10);

      glActiveTexture(GL_TEXTURE10);
      glBindTexture(GL_TEXTURE_2D, lights.at(0)->getDepthMapAsReference());

      glUniform1f(gammaUniform, GAMMA_CORRECTION);

      skipVertexIndex = 0;
      skipTextureIndex = 0;
      skipBumpIndex = 0;

      for (auto & object : objects) {
         //modelM_L2W = object.getWorldCoordinates();

         //glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, modelM_L2W.getArray());
         //glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, object.getLocal2WorldMatrix().getArray());
         glUniformMatrix4fv(modelMatrixUniform, 1, GL_TRUE, object.getWorldCoordinates().getArray());

         for (auto j = 0; j < object.getMeshes().size(); ++j) {
            if (object.doesHaveTextures()) {
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
               glBindTexture(GL_TEXTURE_2D, textureUniforms.at(skipTextureIndex++));

               glActiveTexture(GL_TEXTURE1);
               glBindTexture(GL_TEXTURE_2D, bumpUniforms.at(skipBumpIndex++));
            } else {
               glUniform3f(ambientCoefficient, materials.at(materials.size()-1).getAmbientCoeff().getX(),
                           materials.at(materials.size()-1).getAmbientCoeff().getY(),
                           materials.at(materials.size()-1).getAmbientCoeff().getZ());

               //glUniform3f(fixedColorUniform, 0.2f, 0.2f, 0.2f);
            }

            if (object.doesNeedNoCulling()) {
               glDisable(GL_CULL_FACE);
            }

            glBindVertexArray(vertexArrayObjects.at(skipVertexIndex++));
            // Chamata di disegno della primitiva
            glDrawElements(GL_TRIANGLES, object.getMeshes().at(j).getIndices().size(), GL_UNSIGNED_INT, 0);

            if (object.doesNeedNoCulling()) {
               glEnable(GL_CULL_FACE);
            }
         }
      }

      glUseProgram(cardsShader);

      glDisable(GL_CULL_FACE);

      // Blend di trasparenza
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

      glUniformMatrix4fv(cardProjectionMatrix, 1, GL_TRUE, projM_V2C.getArray());
      glUniformMatrix4fv(cardViewMatrix, 1, GL_TRUE, viewM_W2V.getArray());
      glUniform1i(playerIndexUniform, playerIndex);

      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0x00);

      bool hasSelectedCard = false;
      double x, y;
      glfwGetCursorPos(window, &x, &y);

      if (!players.empty()) {
         for (unsigned int pIndex = 0; pIndex < players.size(); ++pIndex) {
            glUniform1i(viewPlayerUniform, pIndex);

            unsigned int selectedIndex = -1;

            if (pIndex < flipCardRenderingIndex) {
               for (size_t i = 0; i < players.at(pIndex).getCards().size(); ++i) {
                  renderCardsInLoop(cardModelM, cardModelMatrix, viewM_V2W, pIndex, i, hasSelectedCard, selectedIndex, x, y);
               }
            } else {
               for (size_t i = players.at(pIndex).getCards().size()-1; i >= 0 && i != std::numeric_limits<size_t>::max(); --i) {
                  renderCardsInLoop(cardModelM, cardModelMatrix, viewM_V2W, pIndex, i, hasSelectedCard, selectedIndex, x, y);
               }
            }

            if (hasSelectedCard && (playerIndex == pIndex)) {
               glDisable(GL_DEPTH_TEST);

               cardModelM = std::move(players.at(pIndex).getCards().at(selectedIndex).getWorldCoordinates(selectedIndex));

               players.at(pIndex).getCards().at(selectedIndex).updateCoords();

               glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8, cardUVArray);

               glUniformMatrix4fv(cardModelMatrix, 1, GL_TRUE, cardModelM.getArray());

               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

               glEnable(GL_DEPTH_TEST);
            }
         }

         // Genero outlining tramite Stencil test
         glUseProgram(colorShader);

         // Disabilito il Depth Test per poter aggiungere varie informazioni o effetti a schermo
         glDisable(GL_DEPTH_TEST);

         // Verrà stampato qualcosa su schermo solo se supera il controllo sottostante
         // Ovvero quando l'oggetto si trova fuori dalla maschera, ovvero il tratto per l'outlining

         glStencilMask(0x00);    // solo in lettura del buffer, non attiva la scrittura
         glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
         glEnable(GL_DEPTH_TEST);

         glUniformMatrix4fv(colorProjectionMatrix, 1, GL_TRUE, projM_V2C.getArray());
         glUniformMatrix4fv(colorViewMatrix, 1, GL_TRUE, viewM_W2V.getArray());

         for (Card& card : players.at(playerIndex).getCards()) {
            if (card.isSelected1()) {
               cardModelM = std::move(card.getLocal2World() * Transform::scaleMatrix4(1.05, 1.05, 1.05));

               glUniformMatrix4fv(colorModelMatrix, 1, GL_TRUE, cardModelM.getArray());

               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

               card.setIsSelected(false);
               break;
            }
         }
      }

      // Da riattivare per permettere le modifiche al buffer (pulizia)
      glStencilMask(0xFF);

      // Downsampling del color buffer MSAA per riportarlo a risoluzione non upscalata
      if (ENABLE_MULTISAMPLING) {
         glBindFramebuffer(GL_READ_FRAMEBUFFER, offlineFrameBuffer);
         glBindFramebuffer(GL_DRAW_FRAMEBUFFER, secondaryFrameBuffer);
         glBlitFramebuffer(0, 0, X_RESOLUTION, Y_RESOLUTION, 0, 0, X_RESOLUTION, Y_RESOLUTION, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
   // TODO complete: adding remaining variables
   // Liberazione della memoria
   for (int i = 0; i < vertexArrayObjects.size(); ++i) {
      glDeleteBuffers(1, &vertexBufferObjects.at(i));
      glDeleteBuffers(1, &elementBufferObjects.at(i));
      glDeleteVertexArrays(1, &vertexArrayObjects.at(i));
   }

   glDeleteBuffers(1, &sEBO);
   glDeleteBuffers(1, &sVBO);
   glDeleteVertexArrays(1, &sVAO);

   glDeleteProgram(shaderProgram);
   glDeleteFramebuffers(1, &offlineFrameBuffer);
}