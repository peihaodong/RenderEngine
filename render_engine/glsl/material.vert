#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 f_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
	vec4 cur_position = vec4(position, 1.0);
	cur_position = ProjectionMatrix * ViewMatrix * ModelMatrix * cur_position;
	gl_Position = cur_position;
	f_color = color;
};