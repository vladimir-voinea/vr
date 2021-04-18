#include "perspective_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
	perspective_camera::perspective_camera(const perspective_camera::settings& settings)
		: camera(settings.position, settings.direction, settings.up, settings.yaw, settings.pitch)
		, m_settings(settings)
	{
	}

	glm::mat4 perspective_camera::get_projection_matrix() const
	{
		return glm::perspective(
			glm::radians(m_settings.fov),
			m_settings.aspect_ratio,
			m_settings.near,
			m_settings.far);
	}
}