#version 330

layout(std140) uniform GlobalMatrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec4 vTextureBounds;
layout(location = 2) in vec4 vColor;
layout(location = 3) in mat4 vModelMatrix;

uniform uvec2 texSize;

out vec2 uv;
out vec4 color;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * vModelMatrix * vec4(vPosition.x * vTextureBounds.z, vPosition.y * vTextureBounds.w, 0.0, 1.0);
	uv = (vec2(vPosition.x * vTextureBounds.z, -vPosition.y * vTextureBounds.w) + vec2(vTextureBounds.x, -vTextureBounds.y)) / texSize;
	color = vColor;
}