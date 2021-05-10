#pragma once

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>

#include "fps_counter.hpp"
#include "input_listener.hpp"
#include "gui.hpp"

#include <main_loop.hpp>

#include <memory>

class preamble
{
public:
	preamble(vr::glfw::window& m_window);
	void initialize();
	void initialize_glew();
	void initialize_controls();

	void run_loop();
	void process_input();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	vr::glfw::mouse m_mouse;
	fps_counter m_fps_counter;
	std::unique_ptr<main_loop> m_main_loop;
	std::unique_ptr<input_listener> m_input_listener;
	std::unique_ptr<gui> m_gui;

	float m_last_timestamp = 0.f;
};