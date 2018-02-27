#version 410 core

out vec4				color;

in vec2					vertex_tex;

uniform sampler2D		uniform_tex;

void main (void)
{
	color = texture(uniform_tex, vertex_tex);
	vec3 average = vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
	color = vec4(average, 1.0);
}
