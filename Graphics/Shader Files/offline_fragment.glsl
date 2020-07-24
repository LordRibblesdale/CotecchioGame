#version 330 core

in vec2 textureUV;
out vec4 fragColor;

uniform float blurValue;

uniform sampler2D offlineRendering;

float delta = 1.0f / 300.0f;   // Offset per spostarsi tra i pixel vicini al pixel sul quale effettuare il blur

vec2 offsets[25] = vec2[] (
    vec2(-2*delta, 2*delta),  vec2(-delta, 2*delta),  vec2(0, 2*delta),  vec2(delta, 2*delta),  vec2(2*delta, 2*delta),
    vec2(-2*delta, delta),    vec2(-delta, delta),    vec2(0, delta),    vec2(delta, delta),    vec2(-delta, delta),
    vec2(-2*delta, 0),        vec2(-delta, 0),        vec2(0, 0),        vec2(delta, 0),        vec2(-delta, 0),
    vec2(-2*delta, -delta),   vec2(-delta, -delta),   vec2(0, -delta),   vec2(delta, -delta),   vec2(-delta, -delta),
    vec2(-2*delta, -2*delta), vec2(-delta, -2*delta), vec2(0, -2*delta), vec2(delta, -2*delta), vec2(-delta, -2*delta)
);

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

    /*
    if (blurValue != 1) {
        for (int i = 0; i < 25; ++i) {
            blurColor += texture(offlineRendering, textureUV.st + offsets[i]) * blurKernel[i];
        }

        fragColor = mix(blurColor, texture2D, blurValue);
    } else {
        fragColor = texture2D;
    }
    */

    fragColor = texture2D;
}