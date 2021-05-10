#pragma once

#include <glm/glm.hpp>

struct translation
{
	glm::vec3 vec;
};

struct rotation
{
	glm::vec3 euler_angles;
};

struct scale
{
	glm::vec3 value = { 1.f, 1.f, 1.f };
};

struct parameters
{
	std::string path;
	translation translation;
	rotation rotation;
	scale scale;
};