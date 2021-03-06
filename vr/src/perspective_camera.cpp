#include "perspective_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
	perspective_camera::perspective_camera(const perspective_camera::settings& settings)
		: camera(settings.position, settings.direction, settings.fov)
		, m_settings(settings)
	{
	}

	glm::mat4 perspective_camera::get_projection_matrix() const
	{
		return glm::perspective(
			glm::radians(m_zoom),
			m_settings.aspect_ratio,
			m_settings.near,
			m_settings.far);
	}
}