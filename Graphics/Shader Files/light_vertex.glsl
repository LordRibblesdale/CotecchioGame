#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec2 textureCoordinates;
layout (location = 2) in vec3 normalVector;

uniform mat4 lightSpaceMatrix;  // Include la trasformazione World->LightCamera e Camera->Clip
uniform mat4 modelMatrix;

void main() {
    // Coordinate nello spazio della luce (light space)
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(position, 1.0f);
}