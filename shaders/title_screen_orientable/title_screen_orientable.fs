#version 410 core

uniform vec2 uniform_resolution;
uniform float uniform_time;
uniform mat4 uniform_inv_total_matrix;
uniform vec4 uniform_viewport;

out vec4 color;

void main(void)
{
    //Window space to world space conversion
    vec4 ndcPos;

    ndcPos.xy = ((2.0 * gl_FragCoord.xy) - (2.0 * uniform_viewport.xy)) / (uniform_viewport.zw) - 1;
    ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
        (gl_DepthRange.far - gl_DepthRange.near);
    ndcPos.w = 1.0;
    vec4 clipPos = ndcPos / gl_FragCoord.w;
    vec4 eyePos = uniform_inv_total_matrix * clipPos;

    //Actual Shader
    vec2 center = vec2(0.5, 0.5);
    float speed = 0.02;
    float frame_time = 0.013;

    vec2 uv = (eyePos.xy + vec2(1.0, 1.0)) / vec2(2.0, 2.0);

    float fishEyeCorrection = uniform_resolution.y / uniform_resolution.x;

    vec3 base_color = vec3(uv, 0.5 + 0.5 * sin(uniform_time));

	vec2 coord = vec2(center.x - uv.x, (center.y - uv.y) * fishEyeCorrection);
	float radius = -length(coord);
	float z = 1.0 + 0.5 * sin((radius + uniform_time * speed) / frame_time);
    vec3 ripple_color = vec3(z);

	color = vec4(base_color * ripple_color, 1.0);
}
