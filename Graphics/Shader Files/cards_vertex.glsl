#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 normal;

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
    vec4 tmp = /* * */vec4(vertexPosition, 1.0f);
    sPos = vec3(tmp);
    gl_Position = /*projection * view * */tmp;   // Posizione in Clip Space/NDC

    outNormalVector = normalize(mat3(transpose(inverse(model))) * normal);
    outTextCoord = inUV;
}