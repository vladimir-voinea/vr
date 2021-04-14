#include "mesh.hpp"

namespace vr
{
	mesh::mesh(const mesh::geometry_type* geometry, const shader_material* material, const texture* texture)
		: m_geometry(std::move(geometry))
		, m_material(material)
		, m_texture(texture)
	{ }


	const mesh::geometry_type* mesh::get_geometry() const
	{
		return m_geometry;
	}

	const shader_material* mesh::get_material() const
	{
		return m_material;
	}

	const texture* mesh::get_texture() const
	{
		return m_texture;
	}
}