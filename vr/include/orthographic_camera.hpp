#pragma once

#include "camera.hpp"
#include <glm/glm.hpp>

namespace vr
{

	class orthographic_camera : public camera
	{
	public:
		struct settings
		{
			float left;
			float right;
			float bottom;
			float top;
			float near;
			float far;

			float fov;
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
		};

	public:
		orthographic_camera(const settings& settings);
		~orthographic_camera() override = default;

		glm::mat4 get_projection_matrix() const override;

	private:
		const settings& m_settings;
	};
}