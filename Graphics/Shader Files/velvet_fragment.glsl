#version 330 core

in vec2 outTextCoord;
in vec3 outNormalVector;   // Input della normale per i calcoli di colore
in vec3 sPos;           // Input per determinare l'illuminazione (o ottenuta dal vertex shader o di default interpolato)

out vec4 fragColor;

// Scrittura di variabili uniform per ottenere informazioni delle luci (pos, color e coeff)
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightIntensity;

uniform vec3 ambientCoefficient;
uniform vec3 diffusiveCoefficient;

uniform vec3 eye;

vec3 ambiental;
vec3 diffuse;

// Zona di applicazione del test materiale di Velvet
float velvetOffset = 1.0f / 30.0f;

// TODO change here (copia del vettore da learnopengl unicamente per scopi di test)
vec2 offsets[9] = vec2[] (
vec2(-velvetOffset,  velvetOffset), // top-left
vec2( 0.0f,    velvetOffset), // top-center
vec2( velvetOffset,  velvetOffset), // top-right
vec2(-velvetOffset,  0.0f),   // center-left
vec2( 0.0f,    0.0f),   // center-center
vec2( velvetOffset,  0.0f),   // center-right
vec2(-velvetOffset, -velvetOffset), // bottom-left
vec2( 0.0f,   -velvetOffset), // bottom-center
vec2( velvetOffset, -velvetOffset)  // bottom-right
);

float blurNorm = 1.0f / 16.0f;
float blurKernel[9] = float[] (
    blurNorm, 2*blurNorm, blurNorm,
    2*blurNorm, 4*blurNorm, 2*blurNorm,
    blurNorm, 2*blurNorm, blurNorm
);

float edgeDetection[9] = float[] (
    -1, -1, -1,
    -1, 8, -1,
    -1, -1, -1
);

uniform float roughness;
uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture

void main() {
    vec4 color = texture(texture1, outTextCoord) * vec4((ambiental + diffuse) * lightColor, 1);

    vec3 newNormal = normalize(outNormalVector);
    vec3 p2l = normalize(lightPos - sPos);

    // Moltiplico più tardi il fattore comune lightColor
    ambiental = ambientCoefficient * lightIntensity;
    diffuse = 0.5f * max(0, abs(dot(newNormal, p2l))) * diffusiveCoefficient * lightIntensity;

    vec3 view = normalize(eye - sPos);

    vec4 blurColor = vec4(0.0f);
    float normalDotView = dot(normal, view);

    for (int i = 0; i < 9; ++i) {
        blurColor += texture(offlineRendering, textureUV.st + offsets[i]) * blurKernel[i];
    }

    fragColor = mix(blurColor, texture2D, blurValue);
}