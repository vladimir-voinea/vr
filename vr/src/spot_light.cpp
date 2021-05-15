#include "spot_light.hpp"

namespace vr
{
	spot_light::spot_light(const light_components& components, const light_attenuation& attenuation, float inner_cutoff_angle, float outer_cutoff_angle, float intensity)
		: point_light(components, attenuation, intensity)
		, m_inner_cutoff_angle(inner_cutoff_angle)
		, m_outer_cutoff_angle(outer_cutoff_angle)
	{}

	spot_light::~spot_light() = default;

	float spot_light::get_inner_cutoff_angle() const
	{
		return m_inner_cutoff_angle;
	}

	void spot_light::set_inner_cutoff_angle(float angle)
	{
		m_inner_cutoff_angle = angle;
	}

	float spot_light::get_outer_cutoff_angle() const
	{
		return m_outer_cutoff_angle;
	}

	void spot_light::set_outer_cutoff_angle(float angle)
	{
		m_outer_cutoff_angle = angle;
	}

	void spot_light::load_uniforms(const uniform_loader_base& loader, unsigned int light_index)
	{
		loader.load_uniform(make_uniform("vr_spot_light.position", get_world_position()));
		loader.load_uniform(make_uniform("vr_spot_light.direction", front() - get_world_position()));

		loader.load_uniform(make_uniform("vr_spot_light.components.ambient", get_components().ambient));
		loader.load_uniform(make_uniform("vr_spot_light.components.diffuse", get_components().diffuse));
		loader.load_uniform(make_uniform("vr_spot_light.components.specular", get_components().specular));

		loader.load_uniform(make_uniform("vr_spot_light.attenuation.constant", get_attenuation().constant));
		loader.load_uniform(make_uniform("vr_spot_light.attenuation.linear", get_attenuation().linear));
		loader.load_uniform(make_uniform("vr_spot_light.attenuation.quadratic", get_attenuation().quadratic));

		loader.load_uniform(make_uniform("vr_spot_light.cutoff_cosine", std::cos(glm::radians(get_inner_cutoff_angle()))));
		loader.load_uniform(make_uniform("vr_spot_light.outer_cutoff_cosine", std::cos(glm::radians(get_outer_cutoff_angle()))));

		loader.load_uniform(make_uniform("vr_spot_light.intensity", get_intensity()));
	}
}