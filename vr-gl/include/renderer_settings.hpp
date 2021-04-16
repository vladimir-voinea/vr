#pragma once

#include <glm/glm.hpp>

namespace vr::gl
{
	struct renderer_settings
	{
		glm::vec3 clear_color;
		bool wireframe_mode = false;
		bool cull_faces = true;
	};
}