#include "input_listener.hpp"

#include <spdlog/spdlog.h>

void input_listener::on_key_event(vr::glfw::key key, vr::glfw::key_action state, vr::glfw::modifiers mods)
{
	spdlog::info("Key event. Key: {0}, state: {1}, modifiers: {2}", key, state, mods);
}

void input_listener::on_position_event(const vr::glfw::mouse_position& position)
{
	spdlog::info("Mouse move event: x = {0}, y = {1}", position.x, position.y);
}

void input_listener::on_button_event(vr::glfw::mouse_button button, vr::glfw::mouse_action action)
{
	spdlog::info("Mouse button event: Button {0}, action {1}", button, action);
}

void input_listener::on_scroll_event(const vr::glfw::mouse_scroll& scroll)
{
	spdlog::info("Mouse scroll event: x offset: {0}, y offset: {1}", scroll.xoffset, scroll.yoffset);
}

void input_listener::on_enter_event(bool entered)
{
	spdlog::info("Mouse inside the window: {0}", entered);
}