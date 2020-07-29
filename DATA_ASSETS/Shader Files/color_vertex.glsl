#version 330 core

layout (location = 0) in vec3 vertexPosition;

// Projection Matrix
uniform mat4 projection;
// View Matrix
uniform mat4 view;
// Model Matrix
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
}