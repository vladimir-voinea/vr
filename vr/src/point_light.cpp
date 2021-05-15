#include "point_light.hpp"

namespace vr
{
	point_light::point_light(const light_components& components, const light_attenuation& attenuation, float intensity)
		: light(intensity)
		, detail::has_light_components(components)
		, detail::has_light_attenuation(attenuation)
	{}

	point_light::~point_light() = default;

	void point_light::load_uniforms(const uniform_loader_base& loader, unsigned int light_index)
	{
		const auto idx = std::to_string(light_index);

		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].position", get_world_position()));

		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].components.ambient", get_components().ambient));
		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].components.diffuse", get_components().diffuse));
		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].components.specular", get_components().specular));

		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].attenuation.constant", get_attenuation().constant));
		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].attenuation.linear", get_attenuation().linear));
		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].attenuation.quadratic", get_attenuation().quadratic));

		loader.load_uniform(make_uniform("vr_point_lights[" + idx + "].intensity", get_intensity()));
	}
}