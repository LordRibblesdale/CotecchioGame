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
         for (rapidxml::xml_node<>* coordinates = position->first_node("Positions"); coordinates; coordinates = coordinates->next_sibling()) {
            if (std::string(coordinates->first_attribute("type")->value()) == "pos") {
               vertices.emplace_back(std::move(Float3(std::stof(coordinates->first_attribute("x")->value()),
                                        std::stof(coordinates->first_attribute("y")->value()),
                                        std::stof(coordinates->first_attribute("z")->value()))));
            } else if (std::string(coordinates->first_attribute("type")->value()) == "col") {
               colors.emplace_back(Color(std::stof(coordinates->first_attribute("x")->value()),
                                      std::stof(coordinates->first_attribute("y")->value()),
                                      std::stof(coordinates->first_attribute("z")->value())));
            }
         }
      }

      file.close();

      return initialise();
   } else {
      std::cout << "Error XML_FILE_INPUT: no default file has been loaded.";
   }

   return EXIT_FAILURE;
}
