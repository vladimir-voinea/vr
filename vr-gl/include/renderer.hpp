#pragma once

#include <renderer_base.hpp>
#include <camera.hpp>
#include <scene.hpp>

namespace vr::gl
{
	class renderer : renderer_base
	{
	public:
		renderer();

		void render(const vr::scene& scene, const vr::camera& camera) override;
	};
}