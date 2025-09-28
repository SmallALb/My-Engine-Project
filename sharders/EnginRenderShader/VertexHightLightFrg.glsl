#version 460 core


in vec4 color;
in vec2 uv;
in vec3 worldPosition;


out vec4 FragColor;
void main() {
    FragColor = color; 
}