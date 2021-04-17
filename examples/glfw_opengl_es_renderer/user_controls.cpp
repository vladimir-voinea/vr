#include "user_controls.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glfw_keyboard_data.hpp>
#include <glfw_util.hpp>

user_controls::user_controls(vr::glfw::window& window, vr::camera& camera,
	vr::glfw::mouse& mouse, vr::glfw::keyboard& keyboard)
	: m_window(window)
	, m_camera(camera)
	, m_mouse(mouse)
	, m_kb(keyboard)
	, m_yaw(glm::pi<float>())
	, m_pitch(0.f)
	, m_mode(mode::stationary)
{
	const auto mouse_pos = m_mouse.get_position();
	const auto window_size = m_window.get_size();
	m_last_moving_position.x = static_cast<double>(window_size.width) / 2;
	m_last_moving_position.y = static_cast<double>(window_size.height) / 2;

	m_mouse.set_position(m_last_moving_position);

	enter_stationary_mode();
}

void user_controls::enter_moving_mode()
{
	m_mode = mode::moving;
	m_mouse.set_position(m_last_moving_position);
	m_mouse.set_mode(vr::glfw::mouse::mode::disabled);
}

void user_controls::enter_stationary_mode()
{
	m_mode = mode::stationary;
	m_last_moving_position = m_mouse.get_position();
	m_mouse.set_mode(vr::glfw::mouse::mode::normal);
}

void user_controls::mode_switch()
{
	if (m_mode == mode::moving)
	{
		enter_stationary_mode();
	}
	else if (m_mode == mode::stationary)
	{
		enter_moving_mode();
	}
}

void user_controls::process_events(float time_difference)
{

	if (m_mode == mode::moving)
	{
		moving_process_events(time_difference);
	}

	if (m_kb.get_key_state(vr::glfw::key::left_shift) == vr::glfw::key_action::press)
	{
		mode_switch();
	}

	auto view_matrix = glm::lookAt(m_camera.get_position(),
		m_camera.get_position() + m_camera.get_direction(),
		glm::cross(get_right_vector(), m_camera.get_direction()));

	m_camera.set_view_matrix(view_matrix);
}

void user_controls::moving_process_events(float time_difference)
{
	constexpr auto speed = 3.0f;
	constexpr auto mouse_speed = 0.1f;

	const auto mouse_pos = m_mouse.get_position();
	const auto window_size = m_window.get_size();
	m_mouse.set_position({ static_cast<double>(window_size.width) / 2,
		static_cast<double>(window_size.height) / 2 });
	
	m_yaw += mouse_speed *
		time_difference * static_cast<float>(window_size.width / 2 - mouse_pos.x);
	m_pitch += mouse_speed *
		time_difference * static_cast<float>(window_size.height / 2 - mouse_pos.y);

	glm::vec3 direction(
		std::cos(m_pitch) * std::sin(m_yaw),
		std::sin(m_pitch),
		std::cos(m_pitch) * std::cos(m_yaw));
	direction = glm::normalize(direction);

	m_camera.set_direction(direction);

	const auto right = get_right_vector();
	const auto up = glm::cross(right, direction);

	glm::vec3 current_position = m_camera.get_position();
	using k = vr::glfw::key;
	using s = vr::glfw::key_action;
	if (m_kb.get_key_state(k::w) == s::press)
	{
		current_position += direction * time_difference * speed;
	}

	if (m_kb.get_key_state(k::s) == s::press)
	{
		current_position -= direction * time_difference * speed;
	}

	if (m_kb.get_key_state(k::d) == s::press)
	{
		current_position += right * time_difference * speed;
	}

	if (m_kb.get_key_state(k::a) == s::press)
	{
		current_position -= right * time_difference * speed;
	}

	if (m_kb.get_key_state(k::space) == s::press)
	{
		current_position += up * time_difference * speed;
	}

	if (m_kb.get_key_state(k::left_ctrl) == s::press)
	{
		current_position -= up * time_difference * speed;
	}

	m_camera.set_position(current_position);
}

glm::vec3 user_controls::get_right_vector() const
{
	constexpr auto pi_2 = glm::pi<float>() / 2;

	const glm::vec3 right(std::sin(m_yaw - pi_2),
		0,
		std::cos(m_yaw - pi_2));

	return right;
}
