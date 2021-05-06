#include "orthographic_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
	orthographic_camera::orthographic_camera(const orthographic_camera::settings& settings)
		: camera(settings.position, settings.direction, settings.up, settings.yaw, settings.pitch, settings.fov)
		, m_settings(settings)
	{
	}

	glm::mat4 orthographic_camera::get_projection_matrix() const
	{
		return glm::ortho(
			m_settings.left,
			m_settings.right,
			m_settings.bottom,
			m_settings.top,
			m_settings.near,
			m_settings.far);
	}
}