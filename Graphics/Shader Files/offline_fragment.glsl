#version 330 core

in vec2 textureUV;
out vec4 fragColor;

uniform float blurValue;

uniform sampler2D offlineRendering;

float blurOffset = 1.0f / 300.0f;   // Offset per spostarsi tra i pixel vicini al pixel sul quale effettuare il blur

// TODO change here (copia del vettore da learnopengl unicamente per scopi di test)
vec2 offsets[9] = vec2[] (
    vec2(-blurOffset,  blurOffset), // top-left
    vec2( 0.0f,    blurOffset), // top-center
    vec2( blurOffset,  blurOffset), // top-right
    vec2(-blurOffset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( blurOffset,  0.0f),   // center-right
    vec2(-blurOffset, -blurOffset), // bottom-left
    vec2( 0.0f,   -blurOffset), // bottom-center
    vec2( blurOffset, -blurOffset)  // bottom-right
);

float blurNorm = 1.0f / 16.0f;
float blurKernel[9] = float[] (
    blurNorm, 2*blurNorm, blurNorm,
    2*blurNorm, 4*blurNorm, 2*blurNorm,
    blurNorm, 2*blurNorm, blurNorm
);

void main() {
    vec4 texture2D = texture(offlineRendering, textureUV); // Funzione per identificare il colore dalla texture, sampling della texture

    vec4 blurColor = vec4(0.0f);

    for (int i = 0; i < 9; ++i) {
        blurColor += texture(offlineRendering, textureUV.st + offsets[i]) * blurKernel[i];
    }

    fragColor = mix(blurColor, texture2D, blurValue);
}