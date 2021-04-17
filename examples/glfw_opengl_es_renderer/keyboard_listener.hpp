#pragma once

#include <glfw_keyboard_listener.hpp>

class keyboard_listener : public vr::glfw::keyboard_listener
{
public:
	void on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods) override;
};