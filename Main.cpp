#include <rapidxml_utils.hpp>
#include "Graphics/GL_Loader.h"
#include "rapidxml.hpp"

// Argument as XML file importer
int main(int argc, char** argv) {
   // Creazione input buffer
   std::ifstream file("text.xml");

   if (!file.fail()) {
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
      // Iterazioni tra i nodi
      // Creazione dal primo nodo, controllo che esista, imposto il successivo parente
      for (rapidxml::xml_node<>* position = rootNode->first_node("Float3"); position; position = position->next_sibling()) {
         for (rapidxml::xml_node<>* coordinates = position->first_node("Positions"); coordinates; coordinates = coordinates->next_sibling()) {
            array[std::stoi(position->first_attribute("point")->value())]
                    = std::move(Float3(std::stof(coordinates->first_attribute("x")->value()),
                                       std::stof(coordinates->first_attribute("y")->value()),
                                       std::stof(coordinates->first_attribute("z")->value())));
         }
      }

      return initialise();
   }

   return 1;
}
