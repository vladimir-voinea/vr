#include "directional_light.hpp"

namespace vr
{
	directional_light::directional_light(const light_components& components, float intensity)
		: light(intensity)
		, detail::has_light_components(components)
		, m_intensity(intensity)
	{}

	directional_light::~directional_light() = default;

	void directional_light::load_uniforms(const uniform_loader_base& loader, unsigned int light_index)
	{
		loader.load_uniform(make_uniform("vr_directional_light.direction", front() - get_world_position()));
		loader.load_uniform(make_uniform("vr_directional_light.components.ambient", get_components().ambient));
		loader.load_uniform(make_uniform("vr_directional_light.components.diffuse", get_components().diffuse));
		loader.load_uniform(make_uniform("vr_directional_light.components.specular", get_components().specular));
		loader.load_uniform(make_uniform("vr_directional_light.intensity", get_intensity()));
	}
}