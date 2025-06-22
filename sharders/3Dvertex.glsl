#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform vec3 moveUV;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 normat;

out vec4 color;
out vec3 normal;
out vec2 uv;
out vec3 worldPosition;

void main() {
	vec4 transformPosition = vec4(aPos, 1.0);

	transformPosition = model*transformPosition;

	worldPosition = transformPosition.xyz;

   gl_Position = projection* view * transformPosition;
   color = aColor;
   normal =  mat3(normat) * aNormal;
   //normal =  aNormal;

   uv = aUV + moveUV.xy;
}