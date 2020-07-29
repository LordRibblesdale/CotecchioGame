#version 330 core

in vec2 outTextCoord;

out vec4 fragColor;

uniform sampler2D cardTexture;

void main() {
    // Using premultiplied alpha for color bleeding correction
    vec4 tex = texture(cardTexture, outTextCoord);
    fragColor = tex.a * vec4(tex.rgb, 1);
}