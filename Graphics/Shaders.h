#ifndef GLSL_H
#define GLSL_H

#include <memory>

/* IF crea branching, il che varia velocità di render dei pixel
 * Branching è tecnica erronea per la scrittura, poichè causa tempi asincroni per i pixel
 */

static void loadShader(std::string& string, const std::string& location) {
   std::ifstream file(location);
   std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

   string = const_cast<char *>(s.c_str());
}

// Attibute e Uniform
/* Due grandezze, variabili che inserisco nello sharer
 * Attribute: grandezze che rappresentano proprietà ai vertici (in ingresso al vertice, per ogni vertice/elemento a cui lo shader è collegato,
 *             per la singola unità è collegato un vettore "in vec3" -> posizioni
 *             Layout: all'interno del layout si definisce la location, dove definire l'indice del puntatore al vertice che sto considerando
 *
 * Uniform: proprietà "uniforme", grandezza/variabile che rimane fissa (come const) fino a che non viene modificata
 *             ogni unità avrà la stessa proprietà definita dalla variabile uniform
 */

// Impostazione linguaggio GLSL - scrittura direzionalità e tipo di dato

//"layout (location = 1) in vec3 normal;\n" // Definiamo più attributi su piu indici
//"layout (location = 2) in vec2 uv;\n"
//"uniform bool name;\n"   // Valore uniforme, costante per tutti i vertici e/o fragment
//"vec3 pos1 = position.xxx"; // Inizializzazione con accesso a variabili position.xyz con x y z generici
// Attributi: dati

// Fragment -> Pixel  (out per ogni pixel, 4 float definiscono il colore in questo caso RGBA)
#endif //GLSL_H
