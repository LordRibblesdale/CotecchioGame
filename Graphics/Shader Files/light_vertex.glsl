#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main() {
    // Coordinate nello spazio della luce (light space)
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(position, 1.0f);
}