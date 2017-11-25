#version 410 core

out vec4				color;

in vec2					vertex_tex;

uniform sampler2D		uniform_tex_diff;
uniform vec2            uniform_tex_offset;

void main (void)
{
	vec2    vertex_tex_offset;

	vertex_tex_offset = vertex_tex + uniform_tex_offset;
	color = texture(uniform_tex_diff, vertex_tex_offset);
	if (color.a < 0.5f)
	    discard;
}
