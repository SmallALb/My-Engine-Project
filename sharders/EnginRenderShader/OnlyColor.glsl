#version 460 core

out vec4 FragColor;

uniform vec3 InColor;

void main() {
    FragColor = vec4(InColor, 1.0f);
}