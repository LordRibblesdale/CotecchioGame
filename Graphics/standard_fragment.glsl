#version 330 core

in vec2 outTextCoord;
in vec3 normalVector;   // Input della normale per i calcoli di colore
in vec3 sPos;           // Input per determinare l'illuminazione (o ottenuta dal vertex shader o di default interpolato)
in vec4 inColor;        // TODO fix inColor usage

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

// Scrittura di variabili uniform per ottenere informazioni delle luci (pos, color e coeff)
//TODO setup unioform gathering in cpp
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightIntensity;

uniform vec3 ambientCoefficient;
uniform vec3 diffusiveCoefficient;
uniform vec3 specularCoefficient;
uniform float specularAlpha;

uniform vec3 eye;

vec3 ambiental;
vec3 diffuse;
vec3 specular;

uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture


void main() {
    // TODO check if --v possible
    normalVector = normalize(normalVector);
    vec3 p2l = normalize(lightPos * sPos);

    ambiental = ambientCoefficient;
    diffuse = max(0, dot(normalVector, p2l)) * kd * lightIntensity;

    vec3 view = normalize(eye - sPos);
    vec3 reflection = refect(-p2l, normalVector);
    specular = pow(max(0, dot(view, reflection)), specularAlpha) * specularCoefficient;
    fragColor = texture(texture1, outTextCoord) * vec4((ambiental + diffuse + specular) * lightColor, inColor.w);
}