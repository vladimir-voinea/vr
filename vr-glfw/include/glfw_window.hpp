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

		bool close_requested();
		void swap_buffers();
		void set_sticky_keys(bool status);
	private:
		bool create();

	private:
		GLFWwindow* m_window = nullptr;
		const glfw_window_settings m_settings;
	};
}
