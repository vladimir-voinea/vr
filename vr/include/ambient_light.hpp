#pragma once

#include "light.hpp"

namespace vr
{
	class ambient_light : public light
	{
	public:
		ambient_light(const glm::vec3& color, float intensity = 1.f);
		~ambient_light() override;

		const glm::vec3& get_color() const;
		glm::vec3& get_color();

		void set_color(const glm::vec3& color);

		virtual void load_uniforms(const uniform_loader_base& loader, unsigned int light_index) override;

	private:
		glm::vec3 m_color;
	};
}