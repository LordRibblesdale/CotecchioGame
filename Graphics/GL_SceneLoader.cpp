#include "GL_RenderVariables.h"
#include <random>

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/postprocess.h>
#endif

// TODO fix for Unix users
#include <windows.h>

void loadCards() {
   std::vector<unsigned short> values;
   values.reserve(40);

   std::stack<unsigned int> cardsValue;

   SYSTEMTIME time;
   GetSystemTime(&time);
   mt19937_64 gen(time.wMilliseconds + time.wSecond*1000);

   for (auto i = 10; i < 50; ++i) {
      switch (PLAYERS) {
         case 3:
            if (i == 13)
               continue;
            break;
         case 6:
            if ((i - 3) % 10 == 0)
               continue;
            break;
         case 7:
            if ((i - 3) % 10 == 0 || i == 13)
               continue;
            break;
      }

      values.emplace_back(i);
   }

   totalCards = values.size();

   for(unsigned int i = 0; i < 3; ++i) {
      // Problema di seed?
      std::shuffle(values.begin(), values.end(), gen);
   }

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

   // NOTA: dovrà riservare players come size, una carta per giocatore, per via del funzionamento del gioco
   cardsOnTable.reserve(40);
}

void loadObject(const std::string& location, const std::string& objName, const unsigned int& objType) {
   std::string s(location + objName + ".obj");
   const aiScene* scene(importer->ReadFile(s, aiProcess_Triangulate));

   if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "Error ASSIMP_SCENE_LOADING: scene not loaded." << std::endl << importer->GetErrorString() << std::endl;
   } else {
      Model table(location, objName);
      table.processNode(scene->mRootNode, scene);

      objects.emplace_back(std::move(table));
   }

   importer->FreeScene();
}

void loadObjects() {
   importer = std::move(std::make_unique<Assimp::Importer>());

   objects.reserve(2);

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

   for (auto& object : objects) {
      for (const auto& mesh : object.getMeshes()) {
         generateObjects(const_cast<Mesh&>(mesh));
      }

      /* Richiesta della posizione della texture
       * Ricerca dello uniform nello sceneToOfflineRenderingShader necessario, laddove serve caricarlo
         GLuint textureUniform = glGetUniformLocation(sceneToOfflineRenderingShader, "texture1");
       * Assegnazione e modifica uniform a prescindere
       * Uso glUseProgram per assegnare texture
         glUseProgram(sceneToOfflineRenderingShader);
       * Assegnazione valore della texture a uno specifico canale di OpenGL
       * Canali limitati, massimo un certo numero di texture contemporaneamente
         glUniform1i(textureUniform, 0);
         glUseProgram(0);
       */

      loadSettingsNTextures(object.getLocation(), object.getName(), object);
   }

   createPlayerPositions(PLAYERS);
   loadCards();
   loadCardTextures();

   prepareScreenForOfflineRendering();

   prepareCardRendering();

   prepareSceneLights();
}

void generateObjects(Mesh &mesh) {
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
   unsigned int size = 3*mesh.getVertices().size()    //3
           + 2*mesh.getTextureUnwrap().size()         //5
           + 3*mesh.getNormals().size()               //8
           + 3*mesh.getTangents().size()              //11
           + 3*mesh.getBitangents().size();           //14

   GLfloat attributes[size];

   int index = 0;
   for (unsigned int i = 0; i < mesh.getVertices().size(); ++i) {
      attributes[index++] = mesh.getVertices().at(i).getX();
      attributes[index++] = mesh.getVertices().at(i).getY();
      attributes[index++] = mesh.getVertices().at(i).getZ();

      attributes[index++] = mesh.getTextureUnwrap().at(i).getX();
      attributes[index++] = mesh.getTextureUnwrap().at(i).getY();

      attributes[index++] = mesh.getNormals().at(i).getX();
      attributes[index++] = mesh.getNormals().at(i).getY();
      attributes[index++] = mesh.getNormals().at(i).getZ();

      attributes[index++] = mesh.getTangents().at(i).getX();
      attributes[index++] = mesh.getTangents().at(i).getY();
      attributes[index++] = mesh.getTangents().at(i).getZ();

      attributes[index++] = mesh.getBitangents().at(i).getX();
      attributes[index++] = mesh.getBitangents().at(i).getY();
      attributes[index++] = mesh.getBitangents().at(i).getZ();
   }

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, attributes, GL_STATIC_DRAW);
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
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*) (5*sizeof(GLfloat)));
   glEnableVertexAttribArray(2);

   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*) (8*sizeof(GLfloat)));
   glEnableVertexAttribArray(3);

   glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*) (11*sizeof(GLfloat)));
   glEnableVertexAttribArray(4);

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

void prepareCardRendering() {
   glGenVertexArrays(1, &cardVAO);
   glGenBuffers(1, &cardVBO);
   glGenBuffers(1, &cardEBO);

   glBindVertexArray(cardVAO);

   glBindBuffer(GL_ARRAY_BUFFER, cardVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardEBO);

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 32, cardVertices, GL_DYNAMIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, vIndices, GL_DYNAMIC_DRAW);

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
}