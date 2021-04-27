#version 300 es

in vec3 vr_vertex_position;
in vec3 vr_vertex_normal;
in vec4 vr_vertex_color;
in vec2 vr_vertex_uv;

out vec2 uv;

uniform mat4 vr_mvp;
uniform mat4 vr_projection;
uniform mat4 vr_view;
uniform mat4 vr_model;

void main(){

	gl_Position =  vr_mvp * vec4(vr_vertex_position, 1);
	uv = vr_vertex_uv;
}
