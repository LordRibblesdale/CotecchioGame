#ifndef GLSL_H
#define GLSL_H

/* IF crea branching, il che varia velocità di render dei pixel
 * Branching è tecnica erronea per la scrittura, poichè causa tempi asincroni per i pixel
 */

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
const char* vsSource = "#version 330 core\n"
                       "layout (location = 0) in vec3 position;\n" // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
                       "layout (location = 1) in vec3 vertexColor;\n"
                       "out vec3 vertexColor2;\n"
                       "uniform vec3 color;\n"  // Da definire per ottenere informazioni dolore per vertice
                       "void main() {\n"
                       "   gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
                       "   vertexColor2 = vertexColor*color;\n"   // Salvare colore vertici
                       //"   gl_Position = MVP*position\n"  // Posizione in Clip Space/NDC
                       "}\n\0";
//"layout (location = 1) in vec3 normal;\n" // Definiamo più attributi su piu indici
//"layout (location = 2) in vec2 uv;\n"
//"uniform bool name;\n"   // Valore uniforme, costante per tutti i vertici e/o fragment
//"vec3 pos1 = position.xxx"; // Inizializzazione con accesso a variabili position.xyz con x y z generici
// Attributi: dati

// Fragment -> Pixel (out per ogni pixel, 4 float definiscono il colore in questo caso
const char* fsSource = "#version 330 core\n"
                       "in vec3 vertexColor;\n" // Input colore da Vertex Shader
                       "out vec4 fragColor;\n"  // Colore da rappresentare (visualizzato con interpolazione)
                       "void main() {\n"
                       "   fragColor = vec4(vertexColor, 1.0f);\n" // Colore [0,1] RGBA
                       "}\n\0";

#endif //GLSL_H
