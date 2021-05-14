R"(
#version 300 es

in vec3 vr_vertex_position;
in vec3 vr_vertex_normal;
in vec3 vr_vertex_tangent;
in vec3 vr_vertex_bitangent;
in vec4 vr_vertex_color;
in vec2 vr_vertex_uv;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_position;
out mat3 tbn;

uniform vec3 vr_view_position;
uniform mat4 vr_mvp;
uniform mat4 vr_projection;
uniform mat4 vr_view;
uniform mat4 vr_model;
uniform mat4 vr_modelview;
uniform mat3 vr_normal;

void main()
{
	vec4 position4 = vr_model * vec4(vr_vertex_position, 1.f);
	v_position = position4.xyz;
	gl_Position = vr_projection * vr_view * position4;

	v_normal = vr_normal * vr_vertex_normal;
				
	v_uv = vr_vertex_uv;

	vec3 n = normalize((vr_model * vec4(vr_vertex_normal, 0.f)).xyz);
	vec3 t = normalize((vr_model * vec4(vr_vertex_tangent, 0.f)).xyz);
	vec3 b = normalize((vr_model * vec4(vr_vertex_bitangent, 0.f)).xyz);
	tbn = mat3(t, b, n);
}
)"