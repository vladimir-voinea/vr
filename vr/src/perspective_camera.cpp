#include "perspective_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
	perspective_camera::perspective_camera(const perspective_camera::settings& settings)
		: camera()
		, m_settings(settings)
		, Position(m_settings.position)
		, Front(m_settings.direction)
		, WorldUp(m_settings.up)
		, Pitch(m_settings.pitch)
		, Yaw(m_settings.yaw)
	{
		updateCameraVectors();
	}

	glm::mat4 perspective_camera::get_projection_matrix() const
	{
		return glm::perspective(
			glm::radians(m_settings.fov),
			m_settings.aspect_ratio,
			m_settings.near,
			m_settings.far);
	}

	glm::mat4 perspective_camera::get_view_matrix() const
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
}