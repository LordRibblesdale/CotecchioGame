#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria

uniform vec4 inColor;        // Input colore da Vertex Shader
out vec4 outColor;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;

void main() {
    // gl_Position attribuisce la posizione al vertice
    gl_Position = projection * view * model * vec4(position, 1.0f);   // Posizione in Clip Space/NDC

    outColor = inColor;   // Salvare colore vertici
}