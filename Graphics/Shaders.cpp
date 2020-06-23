#include "glad.h"

#include "Shaders.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
//#include <thread>
//#include <mutex>
#include "SceneObjects.h"

#include "rapidxml.hpp"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../IO/stb-master/stb_image.h"

//std::mutex mutex;

void loadShader(std::string &string, const std::string &location) {
   std::ifstream file(location);
   std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

   string = const_cast<char *>(s.c_str());
}

GLuint createTextureUniform() {
   GLuint uniform;
   // Creazione allocazione memoria texture
   glGenTextures(1, &uniform);
   // Bind della texture

   glBindTexture(GL_TEXTURE_2D, uniform);
   /* Impostare come applicare texture su s e t
    *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    * Impostare come comportarsi con dimensioni più o meno piccole in base alla distanza (es usando mipmap)
    */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glBindTexture(GL_TEXTURE_2D, 0);

   return uniform;
}

/*
void loadTextureFromFile(std::vector<GLuint>& textureCoords, std::unordered_map<std::string, GLuint>& map, rapidxml::xml_node<>* position, std::string location) {}

void loadBumpTextureFromFile(std::vector<GLuint> &bumpUniforms, std::unordered_map<std::string, GLuint>& bMap, rapidxml::xml_node<>* bumpNode, std::string location) {}
 */

void loadTexture(const std::string &location, const std::string &name) {
   std::unordered_map<std::string, GLuint> map;
   std::unordered_map<std::string, GLuint> bMap;

   // Creazione input buffer
   std::ifstream file(location + name + ".xml");

   if (file.is_open()) {
      // Creazione documento
      rapidxml::xml_document<> document;
      // Creazione nodo iniziale per lettura file
      rapidxml::xml_node<>* rootNode;
      // Lettura testo :         V: iteratore file stream               V: iteratore fino all'EOF
      std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
      // Aggiunta del carattere EOF \0
      buffer.push_back('\0');
      // Ricerca dei nodi (a partire dall'inizio
      document.parse<0>(&buffer[0]);
      // Ricerca del primo nodo
      rootNode = document.first_node(name.c_str());

      /* Ottenimento matrice dei pixel (1 byte, 8 bit per canale) per valori da 0 a 255, come i PNG 8bit per channel, tramite stb_load)
       * ATTENZIONE nella lettura della texture: In base all'orientamento dell'oggetto, bisogna leggere il file in modo diverso
       * Es: oggetto dal basso verso l'alto, e le immagini dall'alto verso il basso, per un corretto riempimento del buffer
       */
      stbi_set_flip_vertically_on_load(true); // Per leggere il file nell'ordine corretto

      //std::thread thread1;
      //std::thread thread2;

      int width, height, channels;
      unsigned char* data;
      char* texFile;

      GLuint texUniform = 0;

      for (rapidxml::xml_node<>* position = rootNode->first_node("Texture"); position; position = position->next_sibling()) {
         if (std::string("Texture") == position->name()) {
            if (std::string("Empty") != position->value()) {
               texUniform = createTextureUniform();

               texFile = position->first_attribute("name")->value();

               if (map.find(texFile) != map.end()) {
                  textureUniforms.emplace_back(map.at(texFile));
               } else {
                  data = stbi_load((location + texFile).c_str(), &width, &height, &channels, 0);

                  if (data) {
                     /* Analisi dell'immagine, come elaborarla e come farla studiare dalla GPU,
                      *   con informazioni su livelli, canali (es RGBA), dimensioni immagine, formato e formato interno (che dovranno coincidere)
                      *   tipo pixel (GL_UNSIGNED_BYTE), array di pixel
                      */
                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, texUniform);
                     // TODO check channels
                     glTexImage2D(GL_TEXTURE_2D, 0, channels == 3 ? GL_RGB : GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
                     // Creazione della mipmap della texture bindata
                     glGenerateMipmap(GL_TEXTURE_2D);

                     glBindTexture(GL_TEXTURE_2D, 0);
                  } else {
                     std::cout << "Error IMG_LOAD: image not loaded." << std::endl;
                  }

                  stbi_image_free(data);

                  textureUniforms.emplace_back(texUniform);
                  map.emplace(texFile, texUniform);
               }
            } else {
               textureUniforms.emplace_back(0);
            }
         }

         rapidxml::xml_node<>* bumpNode = position->first_node("Bump");
         if (bumpNode) {
            //thread2 = std::move(std::thread(loadBumpTextureFromFile, std::ref(bumpUniforms), std::ref(bMap), bumpNode, location));

            texUniform = createTextureUniform();
            texFile = bumpNode->first_attribute("name")->value();

            if (bMap.find(texFile) != bMap.end()) {
               bumpUniforms.emplace_back(bMap.at(texFile));
            } else {
               data = stbi_load((location + texFile).c_str(), &width, &height, &channels, 0);

               if (data) {
                  /* Analisi dell'immagine, come elaborarla e come farla studiare dalla GPU,
                   *   con informazioni su livelli, canali (es RGBA), dimensioni immagine, formato e formato interno (che dovranno coincidere)
                   *   tipo pixel (GL_UNSIGNED_BYTE), array di pixel
                   */
                  glActiveTexture(GL_TEXTURE1);
                  glBindTexture(GL_TEXTURE_2D, texUniform);
                  // TODO check channels
                  glTexImage2D(GL_TEXTURE_2D, 0, channels == 3 ? GL_RGB : GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
                  // Creazione della mipmap della texture bindata
                  glGenerateMipmap(GL_TEXTURE_2D);

                  glBindTexture(GL_TEXTURE_2D, 0);
               } else {
                  std::cout << texFile << std::endl;
                  std::cout << "Error IMG_LOAD: image not loaded." << std::endl;
               }

               stbi_image_free(data);

               bumpUniforms.emplace_back(texUniform);
               bMap.emplace(texFile, texUniform);
            }
         } else {
            bumpUniforms.emplace_back(0);
         }

         /*
         if (thread1.joinable()) {
            thread1.join();
         }
          */
      }

      //thread2.join();

      file.close();
   } else {
      std::cout << "Error XML_FILE_INPUT: settings file not loaded.";
   }
}

void loadIcon(const std::string& location) {
   GLFWimage icon[1];
   int width, height, channels;
   std::unique_ptr<unsigned char> data(stbi_load(location.c_str(), &width, &height, &channels, 0));
   icon[0].width = width;
   icon[0].height = height;
   icon[0].pixels = data.release();

   if (icon->pixels) {
      glfwSetWindowIcon(window, 1, icon);
   } else {
      std::cout << "Error IMG_LOAD: image not loaded." << std::endl;
   }
}

void loadIcon(const string &location, const string& location2) {
   GLFWimage icon[2];
   int width, height, channels;
   std::unique_ptr<unsigned char> data(stbi_load(location.c_str(), &width, &height, &channels, 0));
   icon[0].width = width;
   icon[0].height = height;
   icon[0].pixels = data.release();
   data.reset(stbi_load(location2.c_str(), &width, &height, &channels, 0));
   icon[1].width = width;
   icon[1].height = height;
   icon[1].pixels = data.release();

   if (icon->pixels) {
      glfwSetWindowIcon(window, 2, icon);
   } else {
      std::cout << "Error IMG_LOAD: image not loaded." << std::endl;
   }
}
