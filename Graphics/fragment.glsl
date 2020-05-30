#version 330 core

in vec4 inColor;        // Input colore da Vertex Shader
in vec2 outTextCoord;
in vec3 normalVector;   // Input della normale per i calcoli di colore

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture

void main() {
    fragColor = texture(texture1, outTextCoord); // Funzione per identificare il colore dalla texture, sampling della texture
}