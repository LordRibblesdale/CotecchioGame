#version 330 core
out vec4 FragColor;

in vec2 textureUV;

uniform sampler2D offlineRendering;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    float depthValue = texture(offlineRendering, textureUV).r;
    //float depth = LinearizeDepth(depthValue);
    //FragColor = vec4(vec3(depth), 1.0);
    FragColor = vec4(vec3(depthValue), 1.0);
}