#include "util.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace vr
{
	matrix_decomposition decompose(const glm::mat4& matrix)
	{
		matrix_decomposition result;

		glm::decompose(matrix, result.scale, result.rotation, result.translation, result.skew, result.perspective);

		return result;
	}
}