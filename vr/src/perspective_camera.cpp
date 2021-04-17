#include "perspective_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
	perspective_camera::perspective_camera(const perspective_camera::settings& settings)
		: camera()
		, m_settings(settings)
	{
		update_projection_matrix();
	}

	void perspective_camera::update_projection_matrix()
	{
		m_projection = glm::perspective(
			glm::radians(m_settings.fov), 
			m_settings.aspect_ratio, 
			m_settings.near, 
			m_settings.far);
	}

	const glm::mat4& perspective_camera::get_projection_matrix() const
	{
		return m_projection;
	}
}