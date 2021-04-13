#pragma once

#include "vertex.hpp"

#include <vector>

namespace vr
{
	struct geometry
	{
		typedef vertex vertex_type;
		typedef uint16_t index_type;

		std::vector<vertex_type> vertices;
		std::vector<index_type> indices;
	};
}