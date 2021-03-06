#include "renderer_cache.hpp"

#include <geometry.hpp>
#include "opengl_shader.hpp"
#include <shader_material.hpp>
#include <texture.hpp>
#include <cube_texture.hpp>

namespace vr::gl
{
	loaded_geometry* renderer_cache::get(const geometry* geometry)
	{
		auto it = m_loaded_geometry.find(geometry);
		return it != m_loaded_geometry.end() ? &it->second : nullptr;
	}

	loaded_shader* renderer_cache::get(const opengl_shader* shader)
	{
		auto it = m_loaded_shaders.find(shader);
		return it != m_loaded_shaders.end() ? &(it->second) : nullptr;
	}

	loaded_texture* renderer_cache::get(const texture* texture)
	{
		auto it = m_loaded_textures.find(texture);
		return it != m_loaded_textures.end() ? &it->second : nullptr;
	}

	loaded_texture* renderer_cache::get(const cube_texture* texture)
	{
		auto it = m_loaded_cube_textures.find(texture);
		return it != m_loaded_cube_textures.end() ? &it->second : nullptr;
	}


	loaded_geometry* renderer_cache::set(const geometry* geometry, loaded_geometry loaded)
	{
		return &m_loaded_geometry.insert({ geometry, std::move(loaded) }).first->second;
	}

	loaded_shader* renderer_cache::set(const opengl_shader* shader, loaded_shader loaded)
	{
		return &m_loaded_shaders.insert({ shader, std::move(loaded) }).first->second;
	}

	loaded_texture* renderer_cache::set(const texture* texture, loaded_texture loaded)
	{
		return &m_loaded_textures.insert({ texture, std::move(loaded) }).first->second;
	}

	loaded_texture* renderer_cache::set(const cube_texture* texture, loaded_texture loaded)
	{
		return &m_loaded_cube_textures.insert({ texture, std::move(loaded) }).first->second;
	}
}