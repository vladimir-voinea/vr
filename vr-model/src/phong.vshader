R"(
#version 300 es

in vec3 vr_vertex_position;
in vec3 vr_vertex_normal;
in vec3 vr_vertex_tangent;
in vec3 vr_vertex_bitangent;
in vec4 vr_vertex_color;
in vec2 vr_vertex_uv;
in vec3 vr_vertex_tangent;
in vec3 vr_vertex_bitangent;

out vec2 v_uv;
out vec3 t_position;
out vec3 t_view_position;

out mat3 vr_tbn;




struct vr_light_components_t
{
	highp vec3 ambient;
	highp vec3 diffuse;
	highp vec3 specular;
};

struct vr_light_attenuation_t
{
	highp float constant;
	highp float linear;
	highp float quadratic;
};

struct vr_directional_light_t
{
	highp vec3 direction;
	vr_light_components_t components;
};

struct vr_point_light_t
{
	highp vec3 position;
	vr_light_attenuation_t attenuation;
	vr_light_components_t components;
};

struct vr_spot_light_t
{
	highp vec3 position;
	highp vec3 direction;
	highp float cutoff_cosine;
	highp float outer_cutoff_cosine;
	vr_light_attenuation_t attenuation;
	vr_light_components_t components;
};


uniform vr_directional_light_t vr_directional_light;
uniform vr_point_light_t vr_point_light;
uniform vr_spot_light_t vr_spot_light;

out vec3 d_d;
out vec3 p_p;
out vec3 s_p;
out vec3 s_d;


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

	d_d = vr_tbn * vr_directional_light.direction;
	p_p = vr_tbn * vr_point_light.position;
	s_p = vr_tbn * vr_spot_light.position;
	s_d = vr_tbn * vr_spot_light.direction;

	vec3 n = normalize((vr_model * vec4(vr_vertex_normal, 0.f)).xyz);
	vec3 t = normalize((vr_model * vec4(vr_vertex_tangent, 0.f)).xyz);
	vec3 b = normalize((vr_model * vec4(vr_vertex_bitangent, 0.f)).xyz);
	tbn = mat3(t, b, n);
}
)"