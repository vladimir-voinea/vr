#pragma once

#include <glm/glm.hpp>

#include <vector>

template<typename geometry_type>
class mesh_base
{
public:
	mesh();
	mesh(geometry_type data_type)
		: m_data(std::move(data_type))
		, m_shader(shader)
	{}

private:
	geometry_type m_data;
}