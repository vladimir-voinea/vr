#include "skybox.hpp"

#include <iterator>
#include <array>

namespace
{
	vr::geometry* generate_cube_geometry()
	{
		constexpr std::array<float, 24> cube_vertices = {
			// front
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0
		};

		constexpr std::array<uint16_t, 36> cube_elements = {
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};

		auto geometry = new vr::geometry{};
		geometry->attributes["vr_vertex_position"].components = 3;
		geometry->attributes["vr_vertex_position"].type = vr::attribute::data_type::t_float;
		const auto begin = reinterpret_cast<const char*>(&cube_vertices[0]);
		const auto end = reinterpret_cast<const char*>(begin + sizeof(cube_vertices));
		std::copy(begin, end, std::back_inserter(geometry->attributes["vr_vertex_position"].data));

		geometry->indices.assign(cube_elements.begin(), cube_elements.end());

		return geometry;
	}
}

namespace vr
{
	skybox::skybox(const shader_material* material, const cube_texture* texture)
		: m_texture(texture)
		, m_geometry(generate_cube_geometry())
		, m_material(material)
	{}

	const cube_texture* skybox::get_texture() const
	{
		return m_texture;
	}

	const geometry* skybox::get_geometry() const
	{
		return m_geometry.get();
	}

	const shader_material* skybox::get_material() const
	{
		return m_material;
	}
}