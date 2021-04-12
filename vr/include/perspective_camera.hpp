#pragma once

#include "camera.hpp"
#include <glm/glm.hpp>

namespace vr
{
	class perspective_camera : public camera
	{
	public:
		struct settings
		{
			float fov;
			float aspect_ratio;
			float near;
			float far;
		};
	public:
		perspective_camera(const settings& settings);

		void set_position(const glm::vec3& position);
		void set_direction(const glm::vec3& direction);

		void update_projection_matrix();
		glm::mat4 get_projection_matrix() const override;

	private:
		settings m_settings;
		glm::mat4 m_projection;
		glm::vec3 m_position;
		glm::vec3 m_direction;
	};
}