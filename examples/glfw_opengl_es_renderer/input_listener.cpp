#include "input_listener.hpp"

#include <vr.hpp>

#include <imgui.h>

#include <spdlog/spdlog.h>

#include <string>

input_listener::input_listener(vr::glfw::window& window, vr::glfw::mouse& mouse, vr::camera& camera, const fps_counter& timing)
	: m_window(window)
	, m_mouse(mouse)
	, m_camera(camera)
	, m_timing(timing)
	, m_state(state::free_mouse)
{
}

void input_listener::on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods)
{
	spdlog::info("Key event. Key: {0}, state: {1}, modifiers: {2}. Listener state: {3}", key, state, mods, m_state);
	auto& io = ImGui::GetIO();

	switch (m_state)
	{
	case state::imgui_input:
	{
		if (io.WantCaptureKeyboard)
		{
			const auto raw_key = vr::glfw::keyboard::convert_to_raw(key);
			const auto pressed = state == vr::glfw::key_action::release ? false : true;;
			
			io.KeysDown[raw_key] = pressed;

			switch (key)
			{
			case vr::glfw::key::left_ctrl:
			{
				io.KeyCtrl = pressed;
				break;
			}
			case vr::glfw::key::left_shift:
			{
				io.KeyShift = pressed;
				break;
			}
			case vr::glfw::key::alt:
			{
				io.KeyAlt = pressed;
				break;
			}
			case vr::glfw::key::super:
			{
				io.KeySuper = pressed;
				break;
			}
			default:
			{
				break;
			}
			}
		}
		break;
	}
	case state::free_mouse:
	{
		// interpolate rotation to align camera with viewing character's rotation
		break;
	}
	case state::camera_move:
	{
		camera_handle_key_event(key, state, mods);
		break;
	}
	}
}

void input_listener::on_char_event(unsigned int codepoint)
{
	spdlog::info("Char event. Codepoint: {0}", codepoint);
	auto& io = ImGui::GetIO();

	switch (m_state)
	{
	case state::imgui_input:
	{
		if (io.WantCaptureKeyboard)
		{
			io.AddInputCharacter(codepoint);
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void input_listener::on_position_event(const vr::glfw::mouse_position& position)
{
	spdlog::info("Mouse move event: x = {0}, y = {1}", position.x, position.y);

	auto& io = ImGui::GetIO();
	io.MousePos.x = position.x;
	io.MousePos.y = position.y;

	switch (m_state)
	{
	case state::camera_move:
	{
		camera_handle_position_event(position);
		break;
	}
	case state::free_mouse:
	{
		if (io.WantCaptureMouse)
		{
			update_state(state::imgui_input);
		}
		break;
	}
	case state::imgui_input:
	{
		if (!io.WantCaptureMouse)
		{
			update_state(state::free_mouse);
		}
		break;
	}
	}

	m_last_mouse_position = position;
}

void input_listener::on_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action)
{
	spdlog::info("Mouse button event: Button {0}, action {1}", button, action);

	auto& io = ImGui::GetIO();
	auto right_button_down = false;

	if (button == vr::glfw::mouse_button::right)
	{
		if (action == vr::glfw::mouse_action::press)
		{
			right_button_down = true;
		}
		else if (action == vr::glfw::mouse_action::release)
		{
			right_button_down = false;
		}
	}

	switch (m_state)
	{
	case state::free_mouse:
	{
		if (right_button_down)
		{
			update_state(state::camera_move);
		}
		break;
	}
	case state::camera_move:
	{
		if (!right_button_down)
		{
			if (io.WantCaptureMouse)
			{
				update_state(state::imgui_input);
			}
			else
			{
				update_state(state::free_mouse);
			}

		}
		break;
	}
	case state::imgui_input:
	{
		if (io.WantCaptureMouse)
		{
			imgui_forward_button_event(button, action);
		}
		else
		{
			update_state(state::free_mouse);
		}
		break;
	}
	}
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

void input_listener::update_state(state new_state)
{
	switch (new_state)
	{
	case state::free_mouse:
	{
		m_mouse.set_mode(vr::glfw::mouse_mode::normal);
		break;
	}
	case state::imgui_input:
	{
		m_mouse.set_mode(vr::glfw::mouse_mode::normal);
		break;
	}
	case state::camera_move:
	{
		m_mouse.set_mode(vr::glfw::mouse_mode::disabled);
		break;
	}
	}

	m_state = new_state;
}

void input_listener::camera_handle_position_event(const vr::glfw::mouse_position& position)
{
	if (!m_last_mouse_position)
	{
		const auto viewport_size = m_window.get_viewport_size();
		m_last_mouse_position = vr::glfw::mouse_position{ static_cast<double>(viewport_size.width / 2), static_cast<double>(viewport_size.height / 2) };

	}
	const float sensitivity = 0.1f;
	const float xoffset = (m_last_mouse_position->x - position.x) * sensitivity;
	const float yoffset = (m_last_mouse_position->y - position.y) * sensitivity;
	spdlog::info("Mouse offsets: x = {0}, y = {1}", xoffset, yoffset);

	m_camera.rotate(vr::x_axis, yoffset);
	m_camera.rotate_world(vr::y_axis, xoffset);
}

void input_listener::imgui_forward_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action)
{
	int imgui_button = 0;

	if (button == vr::glfw::mouse_button::left)
	{
		imgui_button = 0;
	}
	else if (button == vr::glfw::mouse_button::right)
	{
		imgui_button = 1;
	}

	bool imgui_action = false;
	if (action == vr::glfw::mouse_action::press)
	{
		imgui_action = true;
	}

	auto& io = ImGui::GetIO();
	io.MouseDown[imgui_button] = imgui_action;
}

void input_listener::camera_handle_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods)
{
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
