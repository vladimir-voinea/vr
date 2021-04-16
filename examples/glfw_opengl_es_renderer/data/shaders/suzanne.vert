#version 330 core

layout(location = 0) in vec3 vr_vertex_position;
layout(location = 1) in vec3 vr_vertex_normal;
layout(location = 2) in vec4 vr_vertex_color;
layout(location = 3) in vec2 vr_vertex_uv;

out vec2 uv;

uniform mat4 vr_mvp;
uniform mat4 vr_view;
uniform mat4 vr_model;

void main(){

	gl_Position =  vr_mvp * vec4(vr_vertex_position, 1);
	uv = vr_vertex_uv;
}
