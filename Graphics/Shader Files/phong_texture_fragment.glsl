#version 330 core

in vec2 outTextCoord;
in vec3 outNormalVector;   // Input della normale per i calcoli di colore
in vec3 sPos;           // Input per determinare l'illuminazione (o ottenuta dal vertex shader o di default interpolato)

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

// Scrittura di variabili uniform per ottenere informazioni delle luci (pos, color e coeff)
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
uniform sampler2D bumpTexture;  //TODO implement normal mapping

void main() {
    float normalDelta = texture(bumpTexture, outTextCoord).x -0.5f;
    vec3 newNormal = normalize(outNormalVector) + vec3(0, 0, normalDelta);
    vec3 p2l = normalize(lightPos - sPos);

    // Moltiplico più tardi il fattore comune lightColor
    ambiental = ambientCoefficient * lightIntensity;
    diffuse = max(0, abs(dot(newNormal, p2l))) * diffusiveCoefficient * lightIntensity;

    vec3 view = normalize(eye - sPos);
    vec3 reflection = reflect(-p2l, newNormal);
    specular = pow(max(0, abs(dot(view, reflection))), specularAlpha) * specularCoefficient;
    fragColor = texture(texture1, outTextCoord) * vec4((ambiental + diffuse + specular) * lightColor, 1);
}