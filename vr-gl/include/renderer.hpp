#pragma once

#include <camera.hpp>
#include <scene.hpp>

namespace vr::gl
{
	class renderer
	{
	public:
		renderer();

		void render(vr::const scene& scene, const vr::camera& camera) override;
	};
}