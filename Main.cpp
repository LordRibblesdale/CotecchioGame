#include <fstream>
#include "Graphics/GL_Loader.h"
#include "rapidxml.hpp"

// Argument as XML file importer
int main(int argc, char** argv) {
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

      }

      file.close();

      return initialise();
   } else {
      std::cout << "Error XML_FILE_INPUT: no default file has been loaded.";
   }

   return EXIT_FAILURE;
}
