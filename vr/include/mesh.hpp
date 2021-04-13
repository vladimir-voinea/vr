#pragma once

#include "geometry.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace vr
{
	class mesh
	{
	public:
		typedef geometry geometry_type;
	public:
		mesh();
		mesh(geometry_type geometry);

		const geometry_type& get_geometry() const;

	private:
		geometry_type m_geometry;
	};
}