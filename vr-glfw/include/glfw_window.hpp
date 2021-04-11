#pragma once

#include "glfw_window_settings.hpp"

#include <string>

struct GLFWwindow;

namespace vr {
	class glfw_window {
	public:
		glfw_window(glfw_window_settings settings);
		~glfw_window();

		glfw_window(const glfw_window&) = delete;
		glfw_window& operator=(const glfw_window&) = delete;

		bool init();

		GLFWwindow* get_handle();

		bool close_requested();
		bool has_focus();

		void swap_buffers();
		void set_sticky_keys(bool status);
		void set_mouse_visibility(bool stattus);
	private:
		bool create();

	private:
		friend void glfw_window_focus_callback(GLFWwindow*, int);
		void window_focus_callback(bool status);
	
	private:
		GLFWwindow* m_window = nullptr;
		const glfw_window_settings m_settings;

		bool m_has_focus;
	};
}
