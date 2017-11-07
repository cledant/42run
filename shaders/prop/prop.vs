#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4		uniform_mat_total;

out                 vec2 vertex_tex;

void main (void)
{
	vec4 pos_prop;

    vertex_tex = tex_coord;
	pos_prop = uniform_mat_total * vec4(pos, 1.0f);
	gl_Position = pos_prop.xyzw;
}
