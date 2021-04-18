#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
	class camera
	{
	public:
		camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw, float pitch, float zoom);

		virtual ~camera();
		virtual glm::mat4 get_projection_matrix() const = 0;
		virtual glm::mat4 get_view_matrix() const;

		void process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch = true);

		void process_mouse_scroll(float yoffset);

		void translate(const glm::vec3& translation);

		const glm::vec3& get_front() const
		{
			return m_front;
		}

		const glm::vec3& get_right() const
		{
			return m_right;
		}
		
		const glm::vec3& get_up() const
		{
			return m_up;
		}

	private:
		void update_camera_vectors();

	protected:
		float m_zoom;

	private:
		// camera Attributes
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_world_up;
		// euler Angles
		float m_yaw;
		float m_pitch;
		// camera options
		float m_movement_speed;
		float m_mouse_sensitivity;
	};
}