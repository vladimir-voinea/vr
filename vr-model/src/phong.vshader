R"(
#version 300 es

in vec3 vr_vertex_position;
in vec3 vr_vertex_normal;
in vec4 vr_vertex_color;
in vec2 vr_vertex_uv;
in vec3 vr_vertex_tangent;
in vec3 vr_vertex_bitangent;

out vec2 v_uv;
out vec3 t_position;
out vec3 t_view_position;

out mat3 vr_tbn;


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
	vec3 v_position = position4.xyz;
	gl_Position = vr_projection * vr_view * position4;

	vec3 tangent = normalize(vr_normal * vr_vertex_tangent);
	vec3 normal = normalize(vr_normal * vr_vertex_normal);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(normal, tangent);

	vr_tbn = transpose(mat3(tangent, bitangent, normal));

	t_position = vr_tbn * v_position;
	t_view_position = vr_tbn * vr_view_position;
				
	v_uv = vr_vertex_uv;
}
)"