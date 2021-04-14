#pragma once

#include <glm/glm.hpp>

namespace vr
{
	struct vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texcoords;
	};
}
