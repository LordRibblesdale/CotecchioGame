#version 330 core

in vec2 textureUV;
out vec4 fragColor;

uniform sampler2D shadowMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    float depth = texture(shadowMap, textureUV).r;
    depth = LinearizeDepth(depth) / far_plane;
    fragColor = vec4(vec3(depth), 1.0);
}