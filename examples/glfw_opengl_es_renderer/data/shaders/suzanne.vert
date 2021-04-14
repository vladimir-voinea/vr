#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_model;
layout(location = 1) in vec2 vertex_uv_model;
layout(location = 2) in vec3 vertex_normal_model;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;
uniform vec3 light_position_world;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  mvp * vec4(vertex_position_model,1);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (m * vec4(vertex_position_model,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( v * m * vec4(vertex_position_model,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( v * vec4(light_position_world,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( v * m * vec4(vertex_normal_model,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	UV = vertex_uv_model;
}
