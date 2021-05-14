#pragma once

#include <string>
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

struct light_components
{
	glm::vec3 ambient = { 1.f, 1.f, 1.f };
	glm::vec3 diffuse = { 1.f, 1.f, 1.f };
	glm::vec3 specular = { 1.f, 1.f, 1.f };
};

struct light_attenuation
{
	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.32f;
};

struct ambient_light
{
	glm::vec3 color = { 1.f, 1.f, 1.f };
	float intensity = 1.f;
};

struct directional_light
{
	glm::vec3 position = { -30.f, 30.f, 30.f };
	light_components components;
};


struct point_light
{
	glm::vec3 position = { 2.f, 3.f, 2.f };
	light_components components;
	light_attenuation attenuation;
};

struct spot_light
{
	glm::vec3 position = { -3.f, 5.f, 2.f };
	light_components components;
	light_attenuation attenuation;
	float cutoff_angle = 12.f;
	float outer_cutoff_angle = 15.f;
};

struct parameters
{
	std::string path;
	translation translation;
	rotation rotation;
	scale scale;

	bool have_ambient_light = true;
	ambient_light ambient_light;

	bool have_directional_light = false;
	directional_light directional_light;

	bool have_point_light = false;
	point_light point_light;

	bool have_spot_light = false;
	spot_light spot_light;

	parameters();
};