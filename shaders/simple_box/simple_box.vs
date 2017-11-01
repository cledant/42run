#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4		mat_total;

out	vec4			vertex_color;

void main (void)
{
	gl_Position = mat_total * vec4(pos.x, pos.y, pos.z, 1.0f);
	vertex_color = vec4(color.x, color.y, color.z, 1.0f);
}
