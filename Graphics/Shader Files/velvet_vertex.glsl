#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec2 textureCoordinates;
layout (location = 2) in vec3 normalVector;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;
out vec3 outNormalVector;
out vec3 sPos;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;

void main() {
    // gl_Position attribuisce la posizione al vertice
    vec4 tmp = model * vec4(position, 1.0f);
    sPos = vec3(tmp);
    gl_Position = projection * view * tmp;   // Posizione in Clip Space/NDC

    outNormalVector = normalize(mat3(transpose(inverse(model))) * normalVector);
    outTextCoord = textureCoordinates;
}