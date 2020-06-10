#version 330 core

in vec4 outColor;        // Input colore da Vertex Shader

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

void main() {
    fragColor = outColor; // Funzione per identificare il colore dalla texture, sampling della texture
}