#include "glad.h"

#include "Shaders.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "SceneObjects.h"
#include "rapidxml.hpp"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "../IO/stb-master/stb_image.h"

int report;

void loadShader(std::string &string, const std::string& location) {
   std::ifstream file(location);
   string = std::move(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
}

bool compileShader(const string &vLocation, const string &fLocation, GLuint &program) {
   /* Creazione dello shader (vertex o fragment)
    * VERTEX SHADER
    * Restituisce GL unsigned int, indice/puntatore dell'oggetto vertex shader creato dalla GPU
    * Operazione su valori binari, invia chiamata sulla scheda grafica
    */

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   // Variabili per il controllo di errori
   char infoLog[512];

   std::string charSource;
   loadShader(charSource, DATA_ASSETS_LOCATION + vLocation);

   if (charSource.empty()) {
      std::cout << "Error VERTEX_FILE_IMPORT" << std::endl;
      return false;
   }

   // Assegnazione codice allo shader (handle), assegnazione char* (codice GLSL da compilare)
   glShaderSource(vertexShader, 1, &charSource.begin().base(), nullptr);
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
      return false;
   }

   loadShader(charSource, DATA_ASSETS_LOCATION + fLocation);

   if (charSource.empty()) {
      std::cout << "Error FRAGMENT_FILE_IMPORT" << std::endl;
      return false;
   }

   /* FRAGMENT SHADER
    * Restituisce GL unsigned int, indice dell'oggetto fragment shader creato dalla GPU
    */
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &charSource.begin().base(), nullptr);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &report);

   if (!report) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_COMPILE_FRAGMENT: " << infoLog << std::endl;
      return false;
   }

   // Creazione contenitore (program), rappresenta la pipeline di rendering (nel senso delle possibilità programmabili dall'utente)
   program = glCreateProgram();

   // Aggiunta del programma dei vari shader
   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);

   // Link del programma, unisce le unità programmabili (shader)
   glLinkProgram(program);

   // Controllo del link programma, status
   glGetProgramiv(program, GL_LINK_STATUS, &report);

   if (!report) {
      glGetProgramInfoLog(program, 512, nullptr, infoLog);

      std::cout << "Error INFOLOG_LINK_PROGRAM: " << infoLog << std::endl;
      return false;
   }

   // Pulizia memoria dopo la compilazione e link
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   return true;
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
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glBindTexture(GL_TEXTURE_2D, 0);

   return uniform;
}

void createTextureUniform(GLuint& texture) {
   // Creazione allocazione memoria texture
   glGenTextures(1, &texture);
   // Bind della texture

   glBindTexture(GL_TEXTURE_2D, texture);
   /* Impostare come applicare texture su s e t
    *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    * Impostare come comportarsi con dimensioni più o meno piccole in base alla distanza (es usando mipmap)
    */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

   glBindTexture(GL_TEXTURE_2D, 0);
}

void loadTextureOnGPU(unsigned char*& data, GLuint& texUniform, int& channels, int& width, int& height) {
   /* Ottenimento matrice dei pixel (1 byte, 8 bit per canale) per valori da 0 a 255, come i PNG 8bit per channel, tramite stb_load)
    * ATTENZIONE nella lettura della texture: In base all'orientamento dell'oggetto, bisogna leggere il file in modo diverso
    * Es: oggetto dal basso verso l'alto, e le immagini dall'alto verso il basso, per un corretto riempimento del buffer
    */

   if (data) {
      /* Analisi dell'immagine, come elaborarla e come farla studiare dalla GPU,
       *   con informazioni su livelli, canali (es RGBA), dimensioni immagine, formato e formato interno (che dovranno coincidere)
       *   tipo pixel (GL_UNSIGNED_BYTE), array di pixel
       */
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texUniform);
      glTexImage2D(GL_TEXTURE_2D, 0, channels == 3 ? GL_RGB : GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
      // Creazione della mipmap della texture bindata
      glGenerateMipmap(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, 0);
   } else {
      std::cout << "Error IMG_LOAD: image not loaded. " << texUniform << std::endl;
   }
}

void loadTexture(const std::string &location, const std::string &name, bool loadFiles) {
   std::unordered_map<std::string, GLuint> map;
   std::unordered_map<std::string, GLuint> bMap;
   std::unordered_map<std::string, unsigned int> materialMap;

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

      GLuint texUniform = 0;

      int width, height, channels;
      unsigned char* data;
      char* texFile;

      for (rapidxml::xml_node<>* position = rootNode->first_node("Material"); position; position = position->next_sibling()) {
         Material material;

         if (std::string("Wood") == position->first_attribute("name")->value()) {
            material.diffuseCoeff = std::move(Float3(
                    std::stof(position->first_node("Diffuse")->first_attribute("valueR")->value()),
                    std::stof(position->first_node("Diffuse")->first_attribute("valueG")->value()),
                    std::stof(position->first_node("Diffuse")->first_attribute("valueB")->value())
                    ));

            material.specularCoeff = std::move(Float3(
                    std::stof(position->first_node("Specular")->first_attribute("valueR")->value()),
                    std::stof(position->first_node("Specular")->first_attribute("valueG")->value()),
                    std::stof(position->first_node("Specular")->first_attribute("valueB")->value())
                    ));

            material.shininess = std::stof(position->first_node("Specular")->first_attribute("specularExp")->value());
         } else if (std::string("Velvet") == position->first_attribute("name")->value()) {
            material.diffuseCoeff = std::move(Float3(1, 1, 1));
            material.roughness = std::stof(position->first_node("PBR")->first_attribute("roughness")->value());
         } else if (std::string("Generic") == position->first_attribute("name")->value()) {
            material.diffuseCoeff = std::move(Float3(1, 1, 1));
         }

         materials.push_back(material);
      }

      if (loadFiles) {
         for (rapidxml::xml_node<>* position = rootNode->first_node("Texture"); position; position = position->next_sibling()) {
            if (std::string("Texture") == position->name()) {
               if (std::string("Empty") != position->value()) {
                  createTextureUniform(texUniform);

                  texFile = position->first_attribute("name")->value();

                  if (map.find(texFile) != map.end()) {
                     textureUniforms.emplace_back(map.at(texFile));
                  } else {
                     data = stbi_load((location + texFile).c_str(), &width, &height, &channels, 0);

                     loadTextureOnGPU(data, texUniform, channels, width, height);
                     stbi_image_free(data);

                     textureUniforms.emplace_back(texUniform);
                     map.emplace(texFile, texUniform);
                  }
               } else {
                  textureUniforms.emplace_back(0);
               }

               if (std::string("Wood") == position->first_attribute("material")->value()) {
                  materialIndices.emplace_back(0);
               } else if (std::string("Velvet") == position->first_attribute("material")->value()) {
                  materialIndices.emplace_back(1);
               }
            }

            rapidxml::xml_node<>* bumpNode = position->first_node("Bump");
            if (bumpNode) {
               createTextureUniform(texUniform);
               texFile = bumpNode->first_attribute("name")->value();

               if (bMap.find(texFile) != bMap.end()) {
                  bumpUniforms.emplace_back(bMap.at(texFile));
               } else {
                  data = stbi_load((location + texFile).c_str(), &width, &height, &channels, 0);

                  loadTextureOnGPU(data, texUniform, channels, width, height);
                  stbi_image_free(data);

                  bumpUniforms.emplace_back(texUniform);
                  bMap.emplace(texFile, texUniform);
               }
            } else {
               bumpUniforms.emplace_back(0);
            }
         }
      } else {
         materialIndices.emplace_back(2);
      }

      file.close();
   } else {
      std::cout << "Error XML_FILE_INPUT_SHADER: settings file not loaded.";
   }
}

void loadCardTextures() {
   int width, height, channels;
   unsigned char* data(stbi_load((DATA_ASSETS_LOCATION + "Cards.png").c_str(), &width, &height, &channels, 0));

   if (data) {
      createTextureUniform(cardTexture);

      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, cardTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);
   }

   stbi_image_free(data);

   data = stbi_load((DATA_ASSETS_LOCATION + "Back.png").c_str(), &width, &height, &channels, 0);

   if (data) {
      createTextureUniform(backCardTexture);

      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, backCardTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);
   }

   stbi_image_free(data);
}

void loadIcon(const std::string& location) {
   GLFWimage icon[1];
   int width, height, channels;
   unsigned char* data(stbi_load(location.c_str(), &width, &height, &channels, 0));
   icon->width = width;
   icon->height = height;
   icon->pixels = data;

   if (icon->pixels) {
      glfwSetWindowIcon(window, 1, icon);
   } else {
      std::cout << "Error IMG_LOAD: image not loaded. [ICON]" << std::endl;
   }
}