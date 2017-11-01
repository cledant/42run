#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4		mat_total;

out	vec3			tex_coord;

void main (void)
{
	vec4 pos_sky;

	tex_coord = pos;
	pos_sky = mat_total * vec4(pos, 1.0f);
	gl_Position = pos_sky.xyzw;
}
