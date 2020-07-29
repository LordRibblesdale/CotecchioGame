#include "GL_Loader.h"
#include "GL_RenderVariables.h"

//#include <ft2build.h>
//#include FT_FREETYPE_H

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
      glBindFramebuffer(GL_FRAMEBUFFER, msaaSupportFrameBuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, offlineRenderBufferObject);
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_LEVEL, GL_DEPTH24_STENCIL8, X_RESOLUTION, Y_RESOLUTION);

      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaOfflineTexture);
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTISAMPLING_LEVEL, GL_RGB, X_RESOLUTION, Y_RESOLUTION, GL_TRUE);

      glBindTexture(GL_TEXTURE_2D, offlineTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X_RESOLUTION, Y_RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
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
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaOfflineTexture, 0);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

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
      glGenFramebuffers(1, &msaaSupportFrameBuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, msaaSupportFrameBuffer);
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

bool compileShaders() {
   if (compileShader("Shader Files/scene_vertex.glsl", "Shader Files/scene_fragment.glsl", sceneToOfflineRenderingShader)
         //--------------------------OFFLINE RENDERING--------------------------------//
         && compileShader("Shader Files/window_vertex.glsl", "Shader Files/window_fragment.glsl", offlineToWindowShader)
         //---------------------------CARD RENDERING--------------------------------//
         && compileShader("Shader Files/cards_vertex.glsl", "Shader Files/cards_fragment.glsl", cardsShader)
         && compileShader("Shader Files/deck_vertex.glsl", "Shader Files/deck_fragment.glsl", deckShader)
         //---------------------------STENCIL RENDERING--------------------------------//
         && compileShader("Shader Files/outliner_vertex.glsl", "Shader Files/outliner_fragment.glsl", outlinerShader)
         //----------------------------SHADOW RENDERING--------------------------------//
         && compileShader("Shader Files/shadow_map_vertex.glsl", "Shader Files/shadow_map_fragment.glsl", shadowMapShader)
         //------------------------------DEBUG SHADER----------------------------------//
         && compileShader("Shader Files/window_vertex.glsl", "Shader Files/debug_fragment.glsl", debugShader))
   {
      return true;
   } else {
      std::cout << "Error COMPILE_SHADER_FUNCTION: look above for errors." << std::endl;
      return false;
   }
}

void prepareScreenForOfflineRendering() {
   glGenVertexArrays(1, &screenVAO);
   glGenBuffers(1, &screenVBO);
   glGenBuffers(1, &screenEBO);
   glBindVertexArray(screenVAO);

   glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenEBO);

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

bool setupLightMap() {
   bool status;

   // Genero il buffer per il calcolo della profondità (confronto tra profondità)
   //glGenTextures(1, &light->getDepthMapAsReference());
   glGenTextures(1, &shadowMapTexture);
   //glBindTexture(GL_TEXTURE_2D, light->getDepthMapAsReference());
   glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_QUALITY, SHADOW_QUALITY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); // Ancora da non assegnare
   // Da analizzare come classica texture per l'accesso valori
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // Clamp to border per estendere il risultato fuori dai limiti della mappa sfumando l'effetto della mappa
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, Color(1, 1, 1, 1).getVector().get());

   // Genero il framebuffer per il render della shadowmap
   //glGenFramebuffers(1, &light->getFrameBufferAsReference());
   glGenFramebuffers(1, &shadowMapFrameBuffer);
   //glBindFramebuffer(GL_FRAMEBUFFER, light->getFrameBufferAsReference());
   glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFrameBuffer);

   //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light->getDepthMapAsReference(), 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
   // Disattivo la scrittura e lettura, verrà usato solo per la profondità
   glReadBuffer(GL_NONE);
   glDrawBuffer(GL_NONE);

   status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   return status;
}

void prepareSceneLights() {
   // Temporary generate shadow map visualizer
   glGenVertexArrays(1, &debugScreenVAO);
   glGenBuffers(1, &debugScreenVBO);
   glGenBuffers(1, &debugScreenEBO);

   glBindVertexArray(debugScreenVAO);
   glBindBuffer(GL_ARRAY_BUFFER, debugScreenVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugScreenEBO);

   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 20, debugScreen, GL_DYNAMIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, screenIndices, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   light = std::move(std::make_unique<SpotLight>(
         std::move(Float3(0, 0, 20)),
         Float3(0, 0, 0),
         Color(1, 1, 1),
         10,
         // TODO use these angles smartly
         degree2Radiants(30),
         degree2Radiants(45)));

   if (!setupLightMap()) {
      std::cout << "Error SHADOW_MAP_LIGHT 0: Fragment not created." << std::endl;
   }
}

void render() {
   setupRenderVariables();

   flipCardRenderingIndex = ceil(players.size()/2.0f);

   // Abilito Stencil test per l'outlining
   glEnable(GL_STENCIL_TEST);

   // Imposto le modalità di scrittura dello stencil test
   glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

   // Abilito back culling face (non si elaborano facce non visibili in base alla normale
   glEnable(GL_CULL_FACE);

   while (!glfwWindowShouldClose(window)) {  // semmai la finestra dovesse chiudersi
      /* Gestione degli input e render, eseguiti in senso temporale/strutturato nel codice
       * In base all'ordine dei comandi, modifica lo stato del sistema corrente o successivo
       */

      prevTime = currTime;
      currTime = glfwGetTime();

      pollInput(window);

      selectedCardIndex = MAX_SIZE_T_VALUE;

      light->getCamera()->setLookAt(Float3(0, 15*sinf(currTime), 0));

      renderShadowMap();
      renderSceneObjects();
      renderCards();
      renderCardsOnTable();

      // Downsampling del color buffer MSAA per riportarlo a risoluzione non upscalata
      if (ENABLE_MULTISAMPLING) {
         glBindFramebuffer(GL_READ_FRAMEBUFFER, offlineFrameBuffer);
         glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msaaSupportFrameBuffer);
         glBlitFramebuffer(0, 0, X_RESOLUTION, Y_RESOLUTION, 0, 0, X_RESOLUTION, Y_RESOLUTION, GL_COLOR_BUFFER_BIT, GL_NEAREST);
      }

      // Al cambio di framebuffer, è necessario reimpostarlo, come un normale ciclo
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glClearColor(0, 1, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);


      glUseProgram(debugShader);

      glBindVertexArray(debugScreenVAO);
      glUniform1f(nearPlaneUniform, light->getCamera()->getNear());
      glUniform1f(farPlaneUniform, light->getCamera()->getFar());
      glUniform1i(debugMapUniform, 2);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      // -------------------------------------------------------------- //

      glUseProgram(offlineToWindowShader);

      if (MENU_TRANSLATION_CAMERA) {
         glUniform1f(blurUniform, blurValue);
      }

      glBindVertexArray(screenVAO);

      // Accediamo alla texture che adesso è il nostro colore, e la assegneremo a schermo
      glUniform1i(offlineFBTextureUniform, 2);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, offlineTexture);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);

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

   std::cout << printOpenGLError() << std::endl;
}

void cleanMemory() {
   // Liberazione della memoria
   for (unsigned int i = 0; i < vertexArrayObjects.size(); ++i) {
      glDeleteBuffers(1, &vertexBufferObjects.at(i));
      glDeleteBuffers(1, &elementBufferObjects.at(i));
      glDeleteVertexArrays(1, &vertexArrayObjects.at(i));
   }

   glDeleteBuffers(1, &screenEBO);
   glDeleteBuffers(1, &screenVBO);
   glDeleteVertexArrays(1, &screenVAO);

   glDeleteBuffers(1, &cardVBO);
   glDeleteBuffers(1, &cardEBO);
   glDeleteVertexArrays(1, &cardVAO);

   glDeleteBuffers(1, &debugScreenVBO);
   glDeleteBuffers(1, &debugScreenEBO);
   glDeleteVertexArrays(1, &debugScreenVAO);

   glDeleteProgram(sceneToOfflineRenderingShader);
   glDeleteProgram(offlineToWindowShader);
   glDeleteProgram(cardsShader);
   glDeleteProgram(deckShader);
   glDeleteProgram(outlinerShader);
   glDeleteProgram(shadowMapShader);
   glDeleteProgram(debugShader);

   glDeleteRenderbuffers(1, &offlineRenderBufferObject);

   glDeleteFramebuffers(1, &offlineFrameBuffer);
   glDeleteFramebuffers(1, &msaaSupportFrameBuffer);
   glDeleteFramebuffers(1, &shadowMapFrameBuffer);

   importer->FreeScene();
   importer.reset();

   objects.clear();
   cardsOnTable.clear();

   materialIndices.clear();
   materials.clear();

   light.reset();

   players.clear();
}