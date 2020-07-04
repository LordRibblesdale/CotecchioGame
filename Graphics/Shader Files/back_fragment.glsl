#version 330 core

// https://www.blenderguru.com/articles/cycles-shader-encyclopedia

in vec2 outBackUV;

out vec4 fragColor;

uniform sampler2D backTexture;

void main() {
    vec4 tex2Color = texture(backTexture, outBackUV);

    // Using premultiplied alpha for color bleeding correction
    fragColor = tex2Color.a * vec4(tex2Color.rgb, 1);
}