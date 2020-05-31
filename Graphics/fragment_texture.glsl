#version 330 core

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

in vec2 outTextCoord;
uniform sampler2D texture1;

void main() {
    fragColor = texture(texture1, outTextCoord); // Funzione per identificare il colore dalla texture, sampling della texture
}