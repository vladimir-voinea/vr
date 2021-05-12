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

struct light
{
	glm::vec3 position;
	glm::vec3 ambient = { 1.f, 1.f, 1.f };
	glm::vec3 diffuse = { 1.f, 1.f, 1.f };
	glm::vec3 specular = { 1.f, 1.f, 1.f };
};

struct parameters
{
	std::string path;
	translation translation;
	rotation rotation;
	scale scale;

	light light;
};