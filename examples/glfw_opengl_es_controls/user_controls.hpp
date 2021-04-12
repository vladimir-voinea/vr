#pragma once

#include <vr-glfw.hpp>
#include <camera.hpp>

class user_controls
{
	enum class mode
	{
		moving,
		stationary
	};

public:
	user_controls(vr::glfw::window& window, vr::camera& camera, vr::glfw::mouse& mouse, vr::glfw::keyboard& keyboard);

	void process_events(float time_difference);

	// until we move this somewhere else
	glm::mat4 get_view_matrix() const;

private:
	glm::vec3 get_right_vector() const;

	void moving_process_events(float time_difference);
	void mode_switch();
	void enter_moving_mode();
	void enter_stationary_mode();

private:
	vr::glfw::window& m_window;
	vr::camera& m_camera;
	vr::glfw::mouse& m_mouse;
	vr::glfw::keyboard m_kb;

	mode m_mode;
	vr::glfw::mouse::position m_last_moving_position;

	float m_horizontal_angle;
	float m_vertical_angle;

	glm::mat4 m_view_matrix;
};
