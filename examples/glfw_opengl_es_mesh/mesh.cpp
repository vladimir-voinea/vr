#include "mesh.hpp"

mesh::mesh(mesh::geometry_type geometry)
	: m_geometry(std::move(geometry))
{ }


const mesh::geometry_type& mesh::get_geometry() const
{
	return m_geometry;
}