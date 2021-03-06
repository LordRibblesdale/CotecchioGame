#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 inUV;

// IMPORTANTE: i nomi delle variabili tra i due shader devono essere uguali se li si vogliono passare da vertex a fragment
out vec2 outTextCoord;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;

uniform int cardValue;

void main() {
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);

    float u = (cardValue%10)*0.1f;
    float v = ((cardValue/10)-1)*0.25f;
    outTextCoord = vec2(u + (0.1f * inUV.x), v + (0.25f * inUV.y));
}