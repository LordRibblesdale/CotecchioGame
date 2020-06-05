#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 normal;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;
out vec4 inColor;
out vec3 normalVector;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;
// Normal Matrix
uniform mat4 normalM;

void main() {
    // gl_Position attribuisce la posizione al vertice
    //gl_Position = projection * view * model * vec4(position, 1.0f);   // Posizione in Clip Space/NDC
    gl_Position = view * model * vec4(position, 1.0f);

    inColor = vertexColor;   // Salvare colore vertici
    outTextCoord = textureCoordinates;
}