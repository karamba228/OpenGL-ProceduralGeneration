#version 440 core

layout(location = 0) in vec3 aPosition;

out vec2 vTextureCoord;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vTextureCoord = aPosition.xy * 0.5 + 0.5;
}