#version 330 core

in vec2 outTextCoord;
in vec3 outNormalVector;   // Input della normale per i calcoli di colore
in vec3 sPos;           // Input per determinare l'illuminazione (o ottenuta dal vertex shader o di default interpolato)

out vec4 fragColor; // Colore da rappresentare - in questo caso tramite

// Scrittura di variabili uniform per ottenere informazioni delle luci (pos, color e coeff)
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform vec3 ambientCoefficient;
uniform vec3 diffusiveCoefficient;
uniform vec3 specularCoefficient;
uniform float specularAlpha;

uniform vec3 eye;

uniform float gammaCorrection;

vec3 ambiental;
vec3 diffuse;
vec3 specular;
vec3 halfway;

uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture
uniform sampler2D bumpTexture;  //TODO implement normal mapping

void main() {
    float normalDelta = texture(bumpTexture, outTextCoord).x -0.25f;
    vec3 newNormal = (1 + 2.5*normalDelta) * outNormalVector;
    vec3 p2l = normalize(lightPos - sPos);

    // Moltiplico più tardi il fattore comune lightColor
    // TODO implement multiple types of light attenuation
    float pointDistance = distance(lightPos, sPos);
    float attenuation = lightIntensity / (pointDistance);

    ambiental = ambientCoefficient * attenuation;
    diffuse = max(0, dot(newNormal, p2l)) * diffusiveCoefficient * attenuation;

    vec3 view = normalize(eye - sPos);
    //Blinn
    halfway = normalize(p2l + view);
    //Phong
    //vec3 reflection = reflect(-p2l, newNormal);
    //specular = pow(max(0, dot(view, reflection)), specularAlpha) * specularCoefficient;
    specular = pow(max(0, dot(halfway, newNormal)), specularAlpha) * specularCoefficient;

    vec4 txIn = texture(texture1, outTextCoord);
    fragColor = vec4(pow(txIn.rgb, vec3(1.0f/gammaCorrection)) * (ambiental + diffuse + specular), txIn.a);
}