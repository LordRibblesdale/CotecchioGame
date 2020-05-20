#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec3 vertexColor;  //TODO use vec4 for ALPHA

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec4 inColor;

void main() {
// gl_Position attribuisce la posizione al vertice
    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
    inColor = vec4(vertexColor, 1.0f);   // Salvare colore vertici
//"   gl_Position = MVP*position\n"  // Posizione in Clip Space/NDC
}