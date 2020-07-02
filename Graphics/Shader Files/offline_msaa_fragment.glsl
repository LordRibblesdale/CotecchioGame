#version 330 core

in vec2 textureUV;
out vec4 fragColor;

uniform float blurValue;

uniform int samples;
uniform sampler2DMS offlineRendering;

float blurOffset = 1.0f / 300.0f;   // Offset per spostarsi tra i pixel vicini al pixel sul quale effettuare il blur

// TODO change here (copia del vettore da learnopengl unicamente per scopi di test)
ivec2 offsets[9] = ivec2[] (
    ivec2(-blurOffset,  blurOffset), // top-left
    ivec2( 0.0f,    blurOffset), // top-center
    ivec2( blurOffset,  blurOffset), // top-right
    ivec2(-blurOffset,  0.0f),   // center-left
    ivec2( 0.0f,    0.0f),   // center-center
    ivec2( blurOffset,  0.0f),   // center-right
    ivec2(-blurOffset, -blurOffset), // bottom-left
    ivec2( 0.0f,   -blurOffset), // bottom-center
    ivec2( blurOffset, -blurOffset)  // bottom-right
);

float blurNorm = 1.0f / 16.0f;
float blurKernel[9] = float[] (
    blurNorm, 2*blurNorm, blurNorm,
    2*blurNorm, 4*blurNorm, 2*blurNorm,
    blurNorm, 2*blurNorm, blurNorm
);

vec4 textureMultiSample(in sampler2DMS textureMS, in ivec2 UVcoord);

void main() {
    vec4 texture2D = textureMultiSample(offlineRendering, textureUV); // Funzione per identificare il colore dalla texture, sampling della texture

    vec4 blurColor = vec4(0.0f);

    if (blurValue != 1) {
        for (int i = 0; i < 9; ++i) {
            blurColor += textureMultiSample(offlineRendering, ivec2(textureUV.st + offsets[i])) * blurKernel[i];
        }

        fragColor = mix(blurColor, texture2D, blurValue);
    } else {
        fragColor = texture2D;
    }
}

vec4 textureMultiSample(in sampler2DMS textureMS, in ivec2 UVcoord) {
    vec4 color;

    for (int i = 0; i < samples; ++i) {
        color += texelFetch(textureMS, UVcoord, i);
    }

    color /= float(samples);

    return color;
}