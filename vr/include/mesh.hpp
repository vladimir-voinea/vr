#pragma once

#include "geometry.hpp"
#include "shader_material.hpp"
#include "texture.hpp"

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
		mesh(const geometry_type* geometry, shader_material* material, const texture* texture);

		const geometry_type* get_geometry() const;
		const shader_material* get_material() const;
		shader_material* get_material();
		const texture* get_texture() const;

	private:
		const geometry_type* m_geometry;
		shader_material* m_material;
		const texture* m_texture;
	};
}