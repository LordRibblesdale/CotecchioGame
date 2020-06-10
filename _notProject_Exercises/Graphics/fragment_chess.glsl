#version 330 core

in vec4 inColor;        // Input colore da Vertex Shader

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

void main() {
    fragColor = inColor; // Funzione per identificare il colore dalla texture, sampling della texture
}