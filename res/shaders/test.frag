#version 460 core

layout(location = 0) in vec3 fColor;

layout(location = 0) out vec4 colorOut;

void main() {
    colorOut = vec4(fColor, 1.0);
}