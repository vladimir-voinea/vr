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
			
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
			glm::vec3 up = glm::vec3(0.f, 0.1f, 0.f);
			float pitch = 0.f;
			float yaw = -90.f;
		};

	public:
		perspective_camera(const settings& settings);
		~perspective_camera() override = default;

		glm::mat4 get_projection_matrix() const override;

	private:
		const settings& m_settings;
};
}