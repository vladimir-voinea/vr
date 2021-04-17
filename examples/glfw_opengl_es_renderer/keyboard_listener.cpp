#include "keyboard_listener.hpp"

#include <spdlog/spdlog.h>

void keyboard_listener::on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods)
{
	spdlog::info("Key event. Key: {0}, state: {1}, modifiers: {2}", key, state, mods);
}