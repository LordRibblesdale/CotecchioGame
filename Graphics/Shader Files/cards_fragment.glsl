#version 330 core

// https://www.blenderguru.com/articles/cycles-shader-encyclopedia

in vec3 outNormalVector;
in vec2 outTextCoord;
in vec3 sPos;
in vec2 outBackUV;

out vec4 fragColor;

uniform sampler2D cardTexture;
uniform sampler2D backTexture;

uniform int currentPlayer;
uniform int viewPlayer;

void main() {
    float k = viewPlayer == currentPlayer ? 0 : 1;
    vec4 tex1Color = texture(cardTexture, outTextCoord);
    vec4 tex2Color = texture(backTexture, outBackUV);

    // Using premultiplied alpha for color bleeding correction
    fragColor = (1 - k) * tex1Color.a * vec4(tex1Color.rgb, 1) + k * tex2Color.a * vec4(tex2Color.rgb, 1);
    //fragColor = vec4(abs(normalize(eye)), 1);
}