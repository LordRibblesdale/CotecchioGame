#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 normal;

out vec2 textureUV;

void main() {
    gl_Position = vec4(vertexPosition, 1); //Point on screen/rectangle

    textureUV = inUV;
}