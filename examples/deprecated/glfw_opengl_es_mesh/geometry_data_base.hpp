#pragma once

#include <glm/glm.hpp>

#include <vector>

template<typename vertex_type, typename uv_type, typename normal_type, typename index_type>
struct geometry_data_base
{
	std::vector<vertex_type> vertices;
	std::vector<uv_type> uvs;
	std::vector<normal_type> normals;
	std::vector<index_type> indices;
};
