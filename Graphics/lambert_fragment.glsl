#version 330 core

in vec2 outTextCoord;
in vec3 normalVector;   // Input della normale per i calcoli di colore
in vec3 sPos;           // Input per determinare l'illuminazione (o ottenuta dal vertex shader o di default interpolato)

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

// Scrittura di variabili uniform per ottenere informazioni delle luci (pos, color e coeff)
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lambertCoefficient;
uniform vec3 lightIntensity;

uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture

void main() {
    // TODO check if --v possible
    normalVector = normalize(normalVector);
    vec3 p2l = normalize(lightPos * sPos);

    // Lambert = lambCoeff/PI * <N,L>
    // TODO check lightColor multiplication
    fragColor = texture(texture1, outTextCoord) * vec4(max(0, dot(normalVector, p2l)) * kd * lightIntensity, 1.0f) * lightColor;
}