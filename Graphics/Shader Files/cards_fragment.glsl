#version 330 core

in vec3 outNormalVector;
in vec2 outTextCoord;
in vec3 sPos;
in vec2 outBackUV;

out vec4 fragColor;

uniform sampler2D cardTexture;
uniform sampler2D backTexture;

uniform vec3 eye;

void main() {
    float k = dot(normalize(eye - sPos), outNormalVector) >= 0 ? 1 : 0;
    vec4 tex1Color = texture(cardTexture, outTextCoord);
    vec4 tex2Color = texture(backTexture, outBackUV);

    // Using premultiplied alpha for color bleeding correction
    fragColor = (1 - k) * tex1Color.a * vec4(tex1Color.rgb, 1) + k * tex2Color.a * vec4(tex2Color.rgb, 1);
}