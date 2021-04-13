#pragma once

#include <geometry.hpp>
#include "raw_shader_material.hpp"

#include <glm/glm.hpp>

#include <vector>

class mesh
{
public:
	typedef vr::geometry geometry_type;
public:
	mesh();
	mesh(geometry_type geometry);

	const geometry_type& get_geometry() const;

private:
	const geometry_type m_geometry;
};