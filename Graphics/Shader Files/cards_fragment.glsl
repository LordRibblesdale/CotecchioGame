#version 330 core

in vec3 outNormalVector;
in vec2 outTextCoord;
in vec3 sPos;

out vec4 fragColor;

uniform sampler2D cardTexture;
uniform sampler2D backTexture;

uniform vec3 eye;

void main() {
    float k = dot(normalize(eye - sPos), outNormalVector) >= 0 ? 1 : 0;
    fragColor = k * texture(cardTexture, outTextCoord) + (1 - k) * texture(backTexture, outTextCoord);
}