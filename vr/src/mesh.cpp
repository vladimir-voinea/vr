#include "mesh.hpp"

namespace vr
{
	mesh::mesh() = default;

	mesh::mesh(const mesh::geometry_type* geometry, shader_material* material, const std::vector<const texture*>* textures)
		: m_geometry(std::move(geometry))
		, m_material(material)
		, m_textures(textures)
	{ }


	const mesh::geometry_type* mesh::get_geometry() const
	{
		return m_geometry;
	}

	const shader_material* mesh::get_material() const
	{
		return m_material;
	}

	shader_material* mesh::get_material()
	{
		return m_material;
	}
}