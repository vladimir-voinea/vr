#include "ambient_light.hpp"

namespace vr
{
	ambient_light::ambient_light(const glm::vec3& color, float intensity)
		: light(intensity)
		, m_color(color)
	{}

	ambient_light::~ambient_light() = default;

	const glm::vec3& ambient_light::get_color() const
	{
		return m_color;
	}

	glm::vec3& ambient_light::get_color()
	{
		return m_color;
	}

	void ambient_light::set_color(const glm::vec3& color)
	{
		m_color = color;
	}

	void ambient_light::load_uniforms(const uniform_loader_base& loader, unsigned int light_index)
	{
		loader.load_uniform(make_uniform("vr_ambient_light.color", get_color()));
		loader.load_uniform(make_uniform("vr_ambient_light.intensity", get_intensity()));
	}
}