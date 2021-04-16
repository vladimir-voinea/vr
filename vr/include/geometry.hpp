#pragma once

#include "vertex.hpp"

#include <vector>

namespace vr
{
	struct attribute
	{
		enum class data_type
		{
			t_float
		};

		std::vector<unsigned char> data;
		uint8_t components;
	};

	struct geometry
	{
		typedef vertex vertex_type;
		typedef uint16_t index_type;

		std::vector<vertex_type> vertices;
		std::vector<index_type> indices;
	};
}