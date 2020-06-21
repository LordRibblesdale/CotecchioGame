#version 330 core

in vec2 textureUV;
out vec4 fragColor;

uniform sampler2D offlineRendering;

void main() {
    fragColor = texture(offlineRendering, textureUV); // Funzione per identificare il colore dalla texture, sampling della texture
}