#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <spdlog/spdlog.h>

namespace vr
{
	camera::camera(glm::vec3 position, glm::vec3 direction, float zoom)
		: m_zoom(zoom)
	{
		transformable::m_position = position;
	}

	glm::mat4 camera::get_view_matrix() const
	{
		return glm::inverse(get_transformation_matrix());
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void camera::process_mouse_scroll(float yoffset)
	{
		m_zoom -= (float)yoffset;
		if (m_zoom < 1.0f)
			m_zoom = 1.0f;
		if (m_zoom > 45.0f)
			m_zoom = 45.0f;
	}
}