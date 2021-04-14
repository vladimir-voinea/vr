#pragma once

#include <renderer_base.hpp>
#include <camera.hpp>
#include <scene.hpp>

#include <memory>

namespace vr::gl
{
	class renderer_cache;

	class renderer : renderer_base
	{
	public:
		renderer();
		~renderer();

		void render(vr::scene& scene, const vr::camera& camera) override;

	private:
		void load_object(object3d* object);

	private:
		void render_object(const vr::object3d* object, const vr::camera& camera);

	private:
		std::unique_ptr<renderer_cache> m_cache;
	};
}