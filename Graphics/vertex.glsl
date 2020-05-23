#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 textureCoordinates;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;
out vec4 inColor;

uniform mat3 matrix;

void main() {
    // gl_Position attribuisce la posizione al vertice
    //"   gl_Position = MVP*position\n"  // Posizione in Clip Space/NDC
    // TODO applicazione delle matrici per le trasformazioni
    gl_Position = vec4(matrix * position, 1.0f);

    inColor = vertexColor;   // Salvare colore vertici
    outTextCoord = textureCoordinates;
}