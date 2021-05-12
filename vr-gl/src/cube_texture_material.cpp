#include "cube_texture_material.hpp"

#include <spdlog/spdlog.h>

namespace
{
	constexpr auto vshader = R"(
		#version 300 es

		in vec3 vr_vertex_position;
		in vec3 vr_vertex_normal;
		in vec4 vr_vertex_color;
		in vec2 vr_vertex_uv;

		uniform mat4 vr_mvp;
		uniform mat4 vr_projection;
		uniform mat4 vr_view;
		uniform mat4 vr_model;

		out highp vec3 cube_texcoords;

		void main() {
			vec4 position = vr_projection * vr_view * vec4(vr_vertex_position, 1.f);
			gl_Position = position.xyww;
			cube_texcoords = vec3(vr_vertex_position.xy, -vr_vertex_position.z);
		}
		)";

	constexpr auto fshader = R"(
		#version 300 es
		
		in highp vec3 cube_texcoords;
		out highp vec4 color;
		uniform samplerCube sampler;

		void main()
		{
			color = texture(sampler, cube_texcoords);
		}
		)";

	vr::gl::opengl_shader& get_cube_shader()
	{
		static vr::gl::opengl_shader shader{ vshader, fshader };
		return shader;
	}
}

namespace vr::gl
{
	cube_texture_material::cube_texture_material()
		: opengl_shader_material(get_cube_shader())
	{
		spdlog::info("Created cube texture material");
	}

	cube_texture_material::~cube_texture_material() = default;
}