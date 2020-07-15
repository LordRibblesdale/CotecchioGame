#version 330 core

in vec2 outTextCoord;
in vec3 outNormalVector;   // Input della normale per i calcoli di colore
in vec3 sPos;           // Input per determinare l'illuminazione (o ottenuta dal vertex shader o di default interpolato)
in vec4 lightBasedPos;

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

float shadow;

uniform sampler2D texture1; // Tipo di uniform della texture, array che contiene texture
uniform sampler2D bumpTexture;  //TODO implement normal mapping
uniform sampler2D depthMap;

uniform vec3 color;

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

    // Calcolo distanza tra (1) superficie/vista e (2) superficie/luce (se 1 > 2, è in ombra)
    // Richiesta della depth map precedentemente scritta nei calcoli precedenti
    // Necessaria poichè non effettuata al passaggio tra vertex e fragment (non è punto interno allo shader per il calcolo della scena
    // Light Projective/Clip Space -> NDC space

    vec3 perspDivide = lightBasedPos.xyz / lightBasedPos.w;
    // Normalizzazione da [-1, 1] a [0, 1] per il confronto con la profondità
    perspDivide *= 0.5f;    //[-1, 1] -> [-0.5, 0.5]
    perspDivide += 0.5f;    //[-0.5, 0.5] -> [0, 1]

    // Controllo distanze tra camera-punto e luce-punto
    // -> Stiamo confrontando se quel punto effettivamente sia visibile, controllando la distanza tra questo fragment e il calcolo sulla depth map
    shadow = perspDivide.z > texture(depthMap, perspDivide.xy).r ? 1 : 0;

    //fragColor = vec4(pow(txIn.rgb, vec3(1.0f/gammaCorrection)) * (ambiental + (1 - shadow)*(diffuse + specular)) + color, txIn.a);

    fragColor = vec4(vec3(texture(depthMap, perspDivide.xy).r), 1);
}