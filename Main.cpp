#include "Graphics/GL_Loader.h"
#include "Graphics/Settings.h"
#include "Graphics/SceneObjects.h"
#include <fstream>

#include "rapidxml.hpp"

// Accesso a variabile protetta permesso alle classi/sottoclassi
// https://en.cppreference.com/w/cpp/language/access#Protected_member_access

// Argument as XML file importer
int main(int argc, char** argv) {
   // TODO load from settings (or another file) all models to be loaded
   // Creazione input buffer

   std::ifstream file("CotecchioSettings.xml");

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
      rootNode = document.first_node("Settings");

      // Iterazioni tra i nodi
      // Creazione dal primo nodo, controllo che esista, imposto il successivo parente
      X_RESOLUTION = std::stoi(rootNode->first_node("Resolution")->first_attribute("x")->value());
      Y_RESOLUTION = std::stoi(rootNode->first_node("Resolution")->first_attribute("y")->value());

      TEXTURE_QUALITY = std::stoi(rootNode->first_node("Texture")->first_attribute("quality")->value());
      SHADOW_QUALITY = std::stoi(rootNode->first_node("Shadows")->first_attribute("quality")->value());

      ENABLE_MULTISAMPLING = std::stoi(rootNode->first_node("MSAA")->first_attribute("enabled")->value()) == 1;
      MULTISAMPLING_LEVEL = std::stoi(rootNode->first_node("MSAA")->first_attribute("quality")->value());

      UNLOCK_FRAMERATE = std::stoi(rootNode->first_node("Framerate")->first_attribute("unlock")->value()) == 1;
      GAMMA_CORRECTION = std::stof(rootNode->first_node("Gamma")->first_attribute("value")->value());

      CAMERA_SENSIBILITY = std::stof(rootNode->first_node("Camera")->first_attribute("sensibility")->value());

      file.close();

      camera.setAspectRatio(static_cast<float>(Y_RESOLUTION) / static_cast<float>(X_RESOLUTION));
      camera.setSensibility(CAMERA_SENSIBILITY);
   } else {
      std::cout << "Error XML_FILE_INPUT: settings file not loaded.";
   }

   return initialise();
}