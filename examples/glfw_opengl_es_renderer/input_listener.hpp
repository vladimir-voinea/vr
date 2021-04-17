#pragma once

#include <glfw_keyboard_listener.hpp>
#include <glfw_mouse_listener.hpp>

class input_listener : public vr::glfw::keyboard_listener, public vr::glfw::mouse_listener
{
public:
	void on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods) override;
	void on_position_event(const vr::glfw::mouse_position& position) override;
	void on_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action) override;
	void on_scroll_event(const vr::glfw::mouse_scroll& scroll) override;
	void on_enter_event(bool entered) override;
};