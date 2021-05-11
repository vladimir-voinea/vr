#pragma once

#include "vertex.hpp"

#include <vector>
#include <unordered_map>
#include <string>

namespace vr
{
	struct attribute
	{
		enum class data_type
		{
			t_float
		};

		std::string name;
		uint8_t components;
		data_type type;
		std::vector<uint8_t> data;
	};

	struct geometry
	{
		std::unordered_map<std::string, attribute> attributes;
		std::vector<uint32_t> indices;
	};
}