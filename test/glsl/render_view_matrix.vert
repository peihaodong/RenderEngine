#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

void main(void)
{
	vec4 position = vec4(aPos, 1.0);
	position = ViewMatrix * ModelMatrix * position;
	gl_Position = position;
	uv = aUV;
};