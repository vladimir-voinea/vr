R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertex_color;

out vec3 fragment_color;

uniform mat4 mvp;

void main()
{
	fragment_color = vertex_color;

	gl_Position = mvp * vec4(position, 1);
}
)" 