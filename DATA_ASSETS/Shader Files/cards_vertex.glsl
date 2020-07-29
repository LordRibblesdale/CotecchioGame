#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 normal;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;
out vec2 outBackUV;
out vec3 outNormalVector;
out vec3 sPos;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;

uniform int cardValue;

void main() {
    vec4 tmp = model * vec4(vertexPosition, 1.0f);
    sPos = vec3(tmp);
    gl_Position = projection * view * tmp;

    outNormalVector = normalize(mat3(transpose(inverse(model))) * normal);
    outBackUV = inUV;

    vec2 backUV;

    float u = (cardValue%10)*0.1f;
    float v = ((cardValue/10)-1)*0.25f;
    outTextCoord = vec2(u + (0.1f * inUV.x), v + (0.25f * inUV.y));
}