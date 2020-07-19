#version 330 core

layout (location = 0) in vec3 position; // Variabile in ingresso di 3 float (vettore), layout da posizione sulla memoria
layout (location = 1) in vec2 textureCoordinates;
layout (location = 2) in vec3 normalVector;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;
out vec3 outNormalVector;
out vec3 sPos;
out vec4 lightBasedPos;

out vec3 outTangent;
out vec3 outBitangent;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;
// Light Space Matrix
uniform mat4 lightSpaceMatrix;

uniform vec3 tangent;
uniform vec3 bitangent;

mat3 normalMatrix;

void main() {
    // gl_Position attribuisce la posizione al vertice
    vec4 tmp = model * vec4(position, 1.0f);
    sPos = vec3(tmp);
    lightBasedPos = lightSpaceMatrix * tmp;
    gl_Position = projection * view * tmp;   // Posizione in Clip Space/NDC

    normalMatrix = mat3(transpose(inverse(model)));
    outNormalVector = normalize(normalMatrix * normalVector);
    outTangent = normalize(normalMatrix * tangent);
    outBitangent = normalize(normalMatrix * bitangent);

    outTextCoord = textureCoordinates;
}