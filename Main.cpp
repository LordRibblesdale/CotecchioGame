#include <fstream>
#include "Graphics/GL_Loader.h"
#include "rapidxml.hpp"

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

// Argument as XML file importer
int main(int argc, char** argv) {
   std::string s("C:\\Users\\Green\\CLionProjects\\CotecchioGame\\Test.fbx");
   Assimp::Importer importer;
   std::unique_ptr<const aiScene> scene(importer.ReadFile(s, aiProcess_Triangulate));

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      return 0;

   /*
   // Creazione input buffer
   std::ifstream file("test.xml");

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
      rootNode = document.first_node("SimpleTriangle");

      // Conteggio dei vertici per calibrare nella memoria la lunghezza dei vari array
      unsigned int resizeIndex = 0;
      for (rapidxml::xml_node<>* position = rootNode->first_node("Float3"); position; position = position->next_sibling()) {
         ++resizeIndex;
      }

      vertices.reserve(resizeIndex);
      colors.reserve(resizeIndex);
      textureUnwrap.reserve(resizeIndex);
      normals.reserve(resizeIndex);

      // Iterazioni tra i nodi
      // Creazione dal primo nodo, controllo che esista, imposto il successivo parente
      for (rapidxml::xml_node<>* position = rootNode->first_node("Float3"); position; position = position->next_sibling()) {
         // TODO add if exists nodes that are accessed
         vertices.emplace_back(std::move(Float3(std::stof(position->first_node("XPos")->value()),
                                                std::stof(position->first_node("YPos")->value()),
                                                std::stof(position->first_node("ZPos")->value()))));

         colors.emplace_back(Color(std::stof(position->first_node("Red")->value()),
                                   std::stof(position->first_node("Green")->value()),
                                   std::stof(position->first_node("Blue")->value()),
                                   std::stof(position->first_node("Alpha")->value())));

         textureUnwrap.emplace_back(std::move(Float2(std::stof(position->first_node("UCoord")->value()),
                                                     std::stof(position->first_node("VCoord")->value()))));

         normals.emplace_back(std::move(Float3(std::stof(position->first_node("XNormal")->value()),
                                          std::stof(position->first_node("YNormal")->value()),
                                          std::stof(position->first_node("ZNormal")->value()))));
      }

      file.close();
      */

      std::cout << "Error XML_FILE_INPUT: no default file has been loaded.";
   } else {
      return initialise();
   }

   return EXIT_FAILURE;
}