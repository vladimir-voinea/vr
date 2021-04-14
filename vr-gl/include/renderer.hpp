#pragma once

#include "gpu_objects.hpp"
#include "renderer_cache.hpp"

#include <renderer_base.hpp>
#include <camera.hpp>
#include <scene.hpp>

#include <map>

namespace vr::gl
{
	class renderer : renderer_base
	{
	public:
		renderer();

		void render(vr::scene& scene, const vr::camera& camera) override;

	private:
		void load_object(object3d* object);
		loaded_geometry load_geometry(const geometry* geometry);
		loaded_shader load_shader(const shader_material* material);
		loaded_texture load_texture(const texture* texture);

		// void buffer_object(object3d* object);
	private:
		void render_object(const vr::object3d* object, const vr::camera& camera);

	private:
		renderer_cache m_cache;
		std::map<const mesh*, object_on_gpu> m_loaded_gpu_objects;
	};
}