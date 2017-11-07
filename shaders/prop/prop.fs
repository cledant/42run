#version 410 core

out vec4				color;

in vec2					vertex_tex;

uniform sampler2D		uniform_tex_diff;

void main (void)
{
	color = texture(uniform_tex_diff, vertex_tex);
	if (color.a < 0.5f)
	    discard;
}
