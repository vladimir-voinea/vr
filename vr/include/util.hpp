#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vr
{
	struct matrix_decomposition
	{
		glm::vec3 scale;
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 skew;
		glm::vec4 perspective;
	};

	matrix_decomposition decompose(const glm::mat4& matrix);
}