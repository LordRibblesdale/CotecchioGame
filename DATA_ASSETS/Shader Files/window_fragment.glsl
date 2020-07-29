#version 330 core

in vec2 textureUV;
out vec4 fragColor;

uniform sampler2D offlineRendering;

uniform float blurValue;

float delta = 1.5f / textureSize(offlineRendering, 0).y;

float blurNorm = 1.0f / 100.0f;
float blurKernel[25] = float[] (
      blurNorm, 2*blurNorm,  4*blurNorm, 2*blurNorm,   blurNorm,
    2*blurNorm, 4*blurNorm,  8*blurNorm, 4*blurNorm, 2*blurNorm,
    4*blurNorm, 8*blurNorm, 16*blurNorm, 8*blurNorm, 4*blurNorm,
    2*blurNorm, 4*blurNorm,  8*blurNorm, 4*blurNorm, 2*blurNorm,
      blurNorm, 2*blurNorm,  4*blurNorm, 2*blurNorm,   blurNorm
);

void main() {
    vec4 texture2D = texture(offlineRendering, textureUV); // Funzione per identificare il colore dalla texture, sampling della texture

    vec4 blurColor = vec4(0.0f);

    if (blurValue != 1) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                blurColor += texture(offlineRendering, textureUV.st + vec2((i -2), (j -2))*delta) * blurKernel[i*5 + j];
            }
        }

        fragColor = mix(blurColor, texture2D, blurValue);
    } else {
        fragColor = texture2D;
    }
}