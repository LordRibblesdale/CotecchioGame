#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 normalVector;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;
out vec4 inColor;
// TODO implementare la normale e la sua relativa trasformazione
out vec3 outNormalVector;
out vec3 sPos;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;
// Normal Matrix
uniform mat4 normal;

void main() {
    // gl_Position attribuisce la posizione al vertice
    vec3 pos = vec3(model * vec4(position, 1.0f));
    sPos = pos;
    gl_Position = projection * view * vec4(pos, 1.0f);   // Posizione in Clip Space/NDC

    outNormalVector = mat3(transpose(inverse(model))) * normalVector;
    inColor = vertexColor;   // Salvare colore vertici
    outTextCoord = textureCoordinates;
}