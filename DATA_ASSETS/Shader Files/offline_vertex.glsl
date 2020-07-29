#version 330 core

layout (location = 0) in vec3 pixelPosition;
layout (location = 1) in vec2 inUV;

out vec2 textureUV;

void main() {
    gl_Position = vec4(pixelPosition.x, pixelPosition.y, 0, 1); //Point on screen/rectangle

    textureUV = inUV;
}