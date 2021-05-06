#pragma once

#include "skybox.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace vr::gl
{
	struct viewport
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};

	struct renderer_settings
	{
		viewport viewport;

		std::unique_ptr<glm::vec3> clear_color;
		std::unique_ptr<skybox> skybox;

		bool wireframe_mode = false;
		bool cull_faces = true;
	};
}