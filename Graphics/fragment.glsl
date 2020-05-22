#version 330 core

in vec4 inColor;    // Input colore da Vertex Shader
in vec2 outTextCoord;

out vec4 fragColor; // Colore da rappresentare (visualizzato con interpolazione)

uniform vec4 color; // Da definire per ottenere informazioni dolore per vertice
uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture

void main() {
    fragColor = texture(texture1, outTextCoord); // Funzione per identificare il colore dalla texture, sampling della texture
    //fragColor = vec4(inColor*color); // Colore [0,1] RGBA
}