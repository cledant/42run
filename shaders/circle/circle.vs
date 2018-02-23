#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 uniform_total_matrix;

void main (void)
{
    gl_Position = uniform_total_matrix * vec4(pos, 1.0);
}