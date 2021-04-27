#version 300 es

in highp vec2 uv;
out highp vec3 color;
uniform sampler2D vr_texture_sampler;

void main()
{
	color = texture(vr_texture_sampler, uv).rgb;
}