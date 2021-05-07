#include "input_listener.hpp"

#include <vr.hpp>

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
		glm::vec3 direction = m_camera.front();

		using k = vr::glfw::key;
		switch (key)
		{
		case k::w:
		{
			direction = m_camera.front();
			break;
		}
		case k::a:
		{
			direction = -m_camera.right();
			break;
		}
		case k::s:
		{
			direction = -m_camera.front();
			break;
		}
		case k::d:
		{
			direction = m_camera.right();
			break;
		}
		case k::space:
		{
			direction = m_camera.up();
			break;
		}
		case k::left_ctrl:
		{
			direction = -m_camera.up();
			break;
		}
		default:
		{
			spdlog::info("No handler for key {}", key);
			return;
		}
		}

		const auto speed = 2.5f;
		const auto delta_time = m_timing.get_time_since_last_frame();
		const auto velocity = speed * delta_time;
		m_camera.translate(direction * velocity);
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
	const float sensitivity = 0.001f;
	const float xoffset = (m_last_mouse_position->x - position.x) * sensitivity;
	const float yoffset = (m_last_mouse_position->y - position.y) * sensitivity;
	m_last_mouse_position = position;
	spdlog::info("Mouse offsets: x = {0}, y = {1}", xoffset, yoffset);

	m_camera.rotate(vr::x_axis, yoffset);
	m_camera.rotate_world(vr::y_axis, xoffset);
}

void input_listener::on_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action)
{
	spdlog::info("Mouse button event: Button {0}, action {1}", button, action);
}

void input_listener::on_scroll_event(const vr::glfw::mouse_scroll& scroll)
{
	spdlog::info("Mouse scroll event: x offset: {0}, y offset: {1}", scroll.xoffset, scroll.yoffset);

	m_camera.process_mouse_scroll(scroll.yoffset);
}

void input_listener::on_enter_event(bool entered)
{
	spdlog::info("Mouse inside the window: {0}", entered);
}