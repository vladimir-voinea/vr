#pragma once

#include <glm/glm.hpp>

#include <vector>

struct model
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<uint16_t> indices;
};
