#version 410 core

in vec2 uniform_resolution;
in float uniform_time;

out vec4 color;

void main(void)
{
    vec2 center = vec2(0.5 , 0.5);
    float speed = 0.02;
    float frame_time = 0.013;

    vec2 uv = gl_FragCoord.xy / uniform_resolution.xy;

    float fishEyeCorrection = uniform_resolution.y / uniform_resolution.x;

    vec3 base_color = vec3(uv, 0.5 + 0.5 * sin(uniform_time));

	vec2 coord = vec2(center.x - uv.x, (center.y - uv.y) * fishEyeCorrection);
	float radius = -length(coord);
	float z = 1.0 + 0.5 * sin((radius + uniform_time * speed) / frame_time);
    vec3 ripple_color = vec3(z);

	color = vec4(base_color * ripple_color , 1.0);
}
