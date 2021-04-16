#pragma once

#include "skybox.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace vr::gl
{
	struct renderer_settings
	{
		std::unique_ptr<glm::vec3> clear_color;
		std::unique_ptr<skybox> skybox;

		bool wireframe_mode = false;
		bool cull_faces = true;
	};
}