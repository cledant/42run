#version 410 core

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
    float useless = uniform_time;

    vec2 center = vec2(0.5, 0.5);
    vec2 uv = (eyePos.xy + vec2(1.0, 1.0)) / vec2(2.0, 2.0);
    vec3 base_color = vec3(0.4, 0.4, 0.4);

    if ((pow(uv.x - center.x, 2) + pow(uv.y - center.x, 2)) > 0.25)
        discard;

	color = vec4(base_color, 1.0);
}
