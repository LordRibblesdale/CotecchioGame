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
      /*
      std::string buffer("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                         "<SimpleTriangle>\n"
                         "    <Float3>\n"
                         "        <Positions x=\"-0.5\" y=\"-0.5\" z=\"0\"/>\n"
                         "        <Colors r=\"1.0f\" g=\"0.0f\" b=\"0.0f\"/>\n"
                         "    </Float3>\n"
                         "    <Float3>\n"
                         "        <Positions x=\"0.5\" y=\"-0.5\" z=\"0\"/>\n"
                         "        <Colors r=\"0.0f\" g=\"1.0f\" b=\"0.0f\"/>\n"
                         "    </Float3>\n"
                         "    <Float3>\n"
                         "        <Positions x=\"0\" y=\"0.5\" z=\"0\"/>\n"
                         "        <Colors r=\"0.0f\" g=\"0.0f\" b=\"1.0f\"/>\n"
                         "    </Float3>\n"
                         "</SimpleTriangle>");
                         */
      // Aggiunta del carattere EOF \0
      //buffer.push_back('\0');
      // Ricerca dei nodi (a partire dall'inizio
      document.parse<0>(&buffer[0]);
      // Ricerca del primo nodo
      rootNode = document.first_node("SimpleTriangle");
      // Iterazioni tra i nodi
      // Creazione dal primo nodo, controllo che esista, imposto il successivo parente

      /*
      unsigned int resizeIndex = 0;
      for (rapidxml::xml_node<>* position = rootNode->first_node("Float3"); position; position = position->next_sibling()) {
         ++resizeIndex;
      }

      vertices.resize(3*resizeIndex);
      colors.resize(3*resizeIndex);
       */

      for (rapidxml::xml_node<>* position = rootNode->first_node("Float3"); position; position = position->next_sibling()) {
         for (rapidxml::xml_node<>* coordinates = position->first_node("Positions"); coordinates; coordinates = coordinates->next_sibling()) {
            // TODO change basing on vertex composition (x, y, z, r, g, b)
            // Ogni vertice come composizione
            std::cout << coordinates << std::endl;
            vertices.push_back(std::stof(coordinates->first_attribute("x")->value()));
            vertices.push_back(std::stof(coordinates->first_attribute("y")->value()));
            vertices.push_back(std::stof(coordinates->first_attribute("z")->value()));
         }

         for (rapidxml::xml_node<>* colorCoordinates = position->first_node("Colors"); colorCoordinates; colorCoordinates = colorCoordinates->next_sibling()) {
            colors.push_back(std::stof(colorCoordinates->first_attribute("r")->value()));
            colors.push_back(std::stof(colorCoordinates->first_attribute("g")->value()));
            colors.push_back(std::stof(colorCoordinates->first_attribute("b")->value()));
         }
      }

      return initialise();
   } else {
      std::cout << "Error XML_FILE_INPUT: no default file has been loaded.";
   }

   return EXIT_FAILURE;
}
