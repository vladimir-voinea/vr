#pragma once

#include "glfw_window_settings.hpp"

#include <string>

struct GLFWwindow;

namespace vr::glfw {
	struct window_size
	{
		int width;
		int height;
	};

	class window {
	public:
		window(window_settings settings);
		~window();

		window(const window&) = delete;
		window& operator=(const window&) = delete;

		bool init();
		GLFWwindow* get_handle();

		void request_close();
		bool close_requested();

		bool has_focus();
		window_size get_size();

		void swap_buffers();
	private:
		bool create();

	private:
		friend void glfw_window_focus_callback(GLFWwindow*, int);
		void window_focus_callback(bool status);
	
	private:
		GLFWwindow* m_window = nullptr;
		const window_settings m_settings;

		bool m_has_focus;
	};
}
