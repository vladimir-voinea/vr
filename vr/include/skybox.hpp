#pragma once

#include "object3d.hpp"
#include "cube_texture.hpp"
#include "shader_material.hpp"
#include "geometry.hpp"

#include <memory>

namespace vr
{
	class skybox_t
	{
	public:
		skybox_t(const shader_material* material, const cube_texture* texture);

		const cube_texture* get_texture() const;
		const geometry* get_geometry() const;
		const shader_material* get_material() const;

	private:
		const cube_texture* m_texture;
		std::unique_ptr<geometry> m_geometry;
		const shader_material* m_material;
	};
}