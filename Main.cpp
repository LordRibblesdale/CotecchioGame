#include <fstream>
#include "Graphics/GL_Loader.h"
#include "rapidxml.hpp"
#include "Vector/Float2.h"

/* Texture: acquisizione di immagine
 * -> Necessità di geometria con coordinate, per assegnare ai pixel le informazioni
 * -> Texture come oggetto strutturato su OpenGL, ovvero un tipo di buffer
 *    -- OpenGL immagazzina texture 2D e 1D e 3D
 * -> Texture MonoDimensionale: analizzabile come valore di una color ramp (con punto iniziale e finale)
 *    - Unico modo di trattare nello shading un ARRAY (accesso al tipo di struttura, come forzatura)
 * -> Texture2D come immagine
 * -> Texture3D o texture volumetrica: serie di immagini 2D, stratificazione di immagini 2D
 *    - Simil-TAC, analisi di volume con applicazione a intervalli di spazio
 *    - Rappresentare di una determinata zona un VOXEL, pixel volumetrico
 *    - Uso di coordinate uvw
 *    ! Texture3D (uvw) o Texture2DArray (i, uv) sono due cose differenti
 *
 * glTexture ND array
 * glTexture ND
 *
 * Texture wrapping: glTextParameter*, oggetto Texture2D di OpenGL, uso GLuint con codice identificativo di una texture
 * -> Wrap: metodo di trasformazione dell'immagine ai limiti dell'immagine (strq)
 *    -- REPEAT (replica texture), MIRRORED_REPEAT (replica con specchiatura)
 *    -- GL_CLAMP_TO_EDGE (replica ultimi valori dei bordi e duplicazione, indice del pixel di u=0-1, v=0-1)
 *    -- CLAMP_TO_BORDER (si prende il colore di riferimento da inserire al di fuori)
 *
 * Texture filtering: filtraggio ovvero ottenimento colore dalla relazione pixel-colore texture
 *    -- NEAREST: il più vicino colore al punto scelto         (pixeled)
 *    -- LINEAR: coordinate dei più vicini e mescola colore    (blurred)
 * -> Linear più comune per comodità o per interpolazione dei valori
 *    --> Operazioni di ingrandimento e riduzione della texture, e si usa un filtraggio specifico
 *          (generalmente nearest per riduzione (minifying), linear per ingrandimento (magnifying))
 *
 * Mipmap: versioni di texture create come unica texture con textures multiple ma a dimensioni a divisioni di 2, per la risoluzione necessaria
 *    -- Artefatti per posizionamento texture nel caso di oggetti molto piccoli per texture più grandi
 *    -- glGenerateMipmaps genera la mipmap per quella texture
 *    --> GL_filter per il blending tra le texture dlele mipmap_MIPMAP_filter per la rappresentazione della texture stessa
 *          - Attenzione con MAG_FILTER (per cui si usa GL_LINEAR), per MIN_FILTER dipende dal filtro scelto nella riga precedente
 *
 * Creazione e caricamento texture: (stb, OpenImageIO, openIO, irfan, libtiff, libjpeg etc...
 * -> Uso libreria "stb" ed stb_image
 *
 */

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
               // TODO change attributes to r, g, b instead of x, y, z
               vertices.emplace_back(std::move(Float3(std::stof(coordinates->first_attribute("x")->value()),
                                        std::stof(coordinates->first_attribute("y")->value()),
                                        std::stof(coordinates->first_attribute("z")->value()))));
            } else if (std::string(coordinates->first_attribute("type")->value()) == "col") {
               colors.emplace_back(Color(std::stof(coordinates->first_attribute("r")->value()),
                                      std::stof(coordinates->first_attribute("g")->value()),
                                      std::stof(coordinates->first_attribute("b")->value())));
            } else if (std::string(coordinates->first_attribute("type")->value()) == "texture") {
               textureUnwrap.emplace_back(std::move(Float2(std::stof(coordinates->first_attribute("x")->value()),
                                                      std::stof(coordinates->first_attribute("y")->value()))));

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
