#pragma once

#include "renderer_settings.hpp"

#include <renderer_base.hpp>
#include <camera.hpp>
#include <scene.hpp>

#include "vr-opengl.h"

#include <memory>

namespace vr::gl
{
	class renderer_cache;

	class renderer : renderer_base
	{
	public:
		renderer(const renderer_settings& settings);
		~renderer();

		void render(vr::scene& scene, const vr::camera& camera) override;

	private:
		void load_object(object3d* object);

	private:
		void render_object(const vr::object3d* object, const vr::camera& camera);

	private:
		const renderer_settings& m_settings;
		std::unique_ptr<renderer_cache> m_cache;
		GLuint m_last_shader_id = 0;
		GLuint m_last_texture_id = 0;
	};
}