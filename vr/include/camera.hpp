#pragma once

#include <object3d.hpp>
#include <glm/glm.hpp>

namespace vr
{
	class camera : public object3d
	{
	public:
		camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw, float pitch, float zoom);

		virtual ~camera();
		virtual glm::mat4 get_projection_matrix() const = 0;
		virtual glm::mat4 get_view_matrix() const;

		void process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch = true);
		void process_mouse_scroll(float yoffset);

		const glm::vec3& get_front() const;
		const glm::vec3& get_right() const;
		const glm::vec3& get_up() const;

		void rotate(const glm::vec3& axis, float angle) override;

	private:
		void update_camera_vectors();
		void update_front_from_angle_axis(const glm::vec3& axis, float angle);
		void update_front_from_yaw_and_pitch();
		void update_right_and_up_vectors();

		void update_yaw_and_pitch_from_front();
		void update_transformable_quaternion();

	protected:
		float m_zoom;

	private:
		// camera Attributes
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