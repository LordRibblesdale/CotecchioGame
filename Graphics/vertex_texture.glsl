#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec2 textureCoordinates;

in mat4 scale;

out vec2 outTextCoord;

void main() {
    gl_Position = scale * vec4(position, 1.0f);

    outTextCoord = textureCoordinates;
}