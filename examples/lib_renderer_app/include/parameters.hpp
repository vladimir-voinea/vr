#pragma once

#include <string>
#include <glm/glm.hpp>

namespace cereal
{
	template<typename Archive>
	void serialize(Archive& archive, glm::vec3& data)
	{
		archive(data.x, data.y, data.z);
	}
}

struct translation
{
	glm::vec3 vec;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(vec);
	}
};

struct rotation
{
	glm::vec3 euler_angles;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(euler_angles);
	}
};

struct scale
{
	glm::vec3 value = { 1.f, 1.f, 1.f };

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(value);
	}
};

struct light_components
{
	glm::vec3 ambient = { 1.f, 1.f, 1.f };
	glm::vec3 diffuse = { 1.f, 1.f, 1.f };
	glm::vec3 specular = { 1.f, 1.f, 1.f };

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(ambient);
		ar(diffuse);
		ar(specular);
	}
};

struct light_attenuation
{
	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.32f;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(constant, linear, quadratic);
	}
};

struct ambient_light
{
	glm::vec3 color = { 1.f, 1.f, 1.f };
	float intensity = 1.f;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(color);
		ar(intensity);
	}
};

struct directional_light
{
	glm::vec3 position = { -30.f, 30.f, 30.f };
	light_components components;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(position);
		ar(components);
	}
};


struct point_light
{
	glm::vec3 position = { 2.f, 3.f, 2.f };
	light_components components;
	light_attenuation attenuation;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(position);
		ar(components);
		ar(attenuation);
	}
};

struct spot_light
{
	glm::vec3 position = { -3.f, 5.f, 2.f };
	light_components components;
	light_attenuation attenuation;
	float cutoff_angle = 12.f;
	float outer_cutoff_angle = 15.f;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(position);
		ar(components);
		ar(attenuation);
		ar(cutoff_angle, outer_cutoff_angle);
	}
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

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(path);
		ar(translation, rotation, scale);
		ar(have_ambient_light, ambient_light);
		ar(have_directional_light, directional_light);
		ar(have_point_light, point_light);
		ar(have_spot_light, spot_light);
	}
};