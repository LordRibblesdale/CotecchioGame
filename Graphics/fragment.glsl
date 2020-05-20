#version 330 core

in vec4 inColor;    // Input colore da Vertex Shader
out vec4 fragColor; // Colore da rappresentare (visualizzato con interpolazione)
uniform vec4 color; // Da definire per ottenere informazioni dolore per vertice

void main() {
    fragColor = vec4(inColor*color); // Colore [0,1] RGBA
}