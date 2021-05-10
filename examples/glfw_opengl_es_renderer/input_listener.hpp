#pragma once

#include <glfw_keyboard_listener.hpp>
#include <glfw_mouse_listener.hpp>
#include <glfw_window.hpp>
#include <glfw_mouse.hpp>
#include <glfw_keyboard.hpp>
#include <object3d.hpp>
#include <unordered_map>

#include "fps_counter.hpp"

#include <optional>

enum class state
{
	imgui_input,
	camera_move,
	free_mouse // initial state
};

enum class direction
{
	forward,
	backward,
	left,
	right,
	up,
	down
};

class input_listener : public vr::glfw::keyboard_listener, public vr::glfw::mouse_listener
{
public:
	input_listener(vr::glfw::window& window, vr::glfw::mouse& mouse, vr::object3d& camera, const fps_counter& timing);

	void on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods) override;
	void on_char_event(unsigned int codepoint) override;
	void on_position_event(const vr::glfw::mouse_position& position) override;
	void on_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action) override;
	void on_scroll_event(const vr::glfw::mouse_scroll& scroll) override;
	void on_enter_event(bool entered) override;

	void frame(float delta_time);
private:

	void update_state(state new_state);

	void camera_handle_position_event(const vr::glfw::mouse_position& position);
	void imgui_forward_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action);
	void camera_handle_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods);

	void camera_move_towards(direction direction);

private:
	vr::glfw::window& m_window;
	vr::glfw::mouse& m_mouse;
	vr::object3d& m_object;
	const fps_counter& m_timing;
	std::optional<vr::glfw::mouse_position> m_last_mouse_position;
	state m_state;
	bool m_left_button_down = false;
	bool m_right_button_down = false;
};