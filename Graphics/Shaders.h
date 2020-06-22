#ifndef GLSL_H
#define GLSL_H

#ifdef _WIN32

#include "glfw3.h"
#endif

#ifdef linux
#include <GLFW/glfw3.h>
#endif

#include <string>
#include <vector>

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
 * Attibute e Uniform
 * Due grandezze, variabili che inserisco nello sharer
 * Attribute: grandezze che rappresentano proprietà ai vertici (in ingresso al vertice, per ogni vertice/elemento a cui lo shader è collegato,
 *             per la singola unità è collegato un vettore "in vec3" -> posizioni
 *             Layout: all'interno del layout si definisce la location, dove definire l'indice del puntatore al vertice che sto considerando
 *
 * Uniform: proprietà "uniforme", grandezza/variabile che rimane fissa (come const) fino a che non viene modificata
 *             ogni unità avrà la stessa proprietà definita dalla variabile uniform


   Impostazione linguaggio GLSL - scrittura direzionalità e tipo di dato

   "layout (location = 1) in vec3 normal;\n" // Definiamo più attributi su piu indici
   "layout (location = 2) in vec2 uv;\n"
   "uniform bool name;\n"   // Valore uniforme, costante per tutti i vertici e/o fragment
   "vec3 pos1 = position.xxx"; // Inizializzazione con accesso a variabili position.xyz con x y z generici
    Attributi: dati

   Fragment -> Pixel  (out per ogni pixel, 4 float definiscono il colore in questo caso RGBA)

 * IF crea branching, il che varia velocità di render dei pixel
 * Branching è tecnica erronea per la scrittura, poichè causa tempi asincroni per i pixel
 */

void loadShader(std::string& string, const std::string& location);

GLuint createTextureUniform();

void loadTexture(const std::string &location, const std::string &name);

void loadIcon(const std::string& location);

#endif //GLSL_H
