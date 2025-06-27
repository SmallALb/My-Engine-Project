#version 460 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D FontTex;
uniform vec3 color;

void main() {
    float alpha = texture(FontTex, uv).r;

    FragColor = vec4(color, alpha);
}
