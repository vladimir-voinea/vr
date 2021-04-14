#pragma once

#include "gpu_objects.hpp"

#include <map>

namespace vr
{
	class geometry;
	class opengl_shader;
	class texture;
}

namespace vr::gl
{
	class renderer_cache
	{
	public:
		renderer_cache() = default;
		~renderer_cache() = default;

		loaded_geometry* get(const geometry*);
		loaded_shader* get(const opengl_shader*);
		loaded_texture* get(const texture*);

		loaded_geometry* set(const geometry*, loaded_geometry);
		loaded_shader* set(const opengl_shader*, loaded_shader);
		loaded_texture* set(const texture*, loaded_texture);

	private:
		std::map<const geometry*, loaded_geometry> m_loaded_geometry;
		std::map<const opengl_shader*, loaded_shader> m_loaded_shaders;
		std::map<const texture*, loaded_texture> m_loaded_textures;
	};
}