#include "input_listener.hpp"

#include <spdlog/spdlog.h>

input_listener::input_listener(vr::glfw::window& window, vr::camera& camera, const fps_counter& timing)
	: m_window(window)
	, m_camera(camera)
	, m_timing(timing)
{}

void input_listener::on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods)
{
	spdlog::info("Key event. Key: {0}, state: {1}, modifiers: {2}", key, state, mods);

	if (state == vr::glfw::key_action::press || state == vr::glfw::key_action::repeat)
	{
		using m = vr::Camera_Movement;
		m movement = m::FORWARD;
		using k = vr::glfw::key;
		switch (key)
		{
		case k::w:
		{
			movement = m::FORWARD;
			break;
		}
		case k::a:
		{
			movement = m::LEFT;
			break;
		}
		case k::s:
		{
			movement = m::BACKWARD;
			break;
		}
		case k::d:
		{
			movement = m::RIGHT;
			break;
		}
		}

		m_camera.ProcessKeyboard(movement, m_timing.get_time_since_last_frame());
	}
}

void input_listener::on_position_event(const vr::glfw::mouse_position& position)
{
	spdlog::info("Mouse move event: x = {0}, y = {1}", position.x, position.y);

	if (!m_last_mouse_position)
	{
		const auto viewport_size = m_window.get_viewport_size();
		m_last_mouse_position = vr::glfw::mouse_position{ static_cast<double>(viewport_size.width / 2), static_cast<double>(viewport_size.height / 2) };

	}

	const float xoffset = position.x - m_last_mouse_position->x;
	const float yoffset = m_last_mouse_position->y - position.y;

	m_last_mouse_position = position;

	m_camera.ProcessMouseMovement(xoffset, yoffset);
}

void input_listener::on_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action)
{
	spdlog::info("Mouse button event: Button {0}, action {1}", button, action);
}

void input_listener::on_scroll_event(const vr::glfw::mouse_scroll& scroll)
{
	spdlog::info("Mouse scroll event: x offset: {0}, y offset: {1}", scroll.xoffset, scroll.yoffset);

	m_camera.ProcessMouseScroll(scroll.yoffset);
}

void input_listener::on_enter_event(bool entered)
{
	spdlog::info("Mouse inside the window: {0}", entered);
}