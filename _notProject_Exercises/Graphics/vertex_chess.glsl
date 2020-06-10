#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec4 vertexColor;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec4 inColor;

void main() {
    // gl_Position attribuisce la posizione al vertice
    //gl_Position = projection * view * model * vec4(position, 1.0f);   // Posizione in Clip Space/NDC
    gl_Position = vec4(position, 1.0f);

    inColor = vertexColor;   // Salvare colore vertici
}