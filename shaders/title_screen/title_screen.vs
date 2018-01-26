#version 410 core

layout (location = 0) in vec3 pos;

void main (void)
{
    gl_Position = vec4(vec3(pos), 1.0f);
}