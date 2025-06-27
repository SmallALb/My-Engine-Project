#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform vec3 moveUV;
uniform vec2 movePos;
out vec2 uv;

void main() {
    gl_Position = vec4(aPos + movePos, 0.0, 1.0);
    uv = aUV + moveUV.xy;
}


