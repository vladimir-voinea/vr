#pragma once

#include "glfw_window_settings.hpp"
#include <functional>
#include <string>
#include <memory>

struct GLFWwindow;

namespace vr::glfw {
	struct user_pointer;
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
		window_size get_viewport_size();

		void set_framebuffer_resize_callback(std::function<void(int, int)> callback);
		void swap_buffers();
	private:
		bool create();

	private:
		friend void glfw_window_focus_callback(GLFWwindow*, int);
		void window_focus_callback(bool status);

		friend void glfw_framebuffer_resize_callback(GLFWwindow*, int, int);
		void window_framebuffer_callback(int width, int height);
	
	private:
		GLFWwindow* m_window = nullptr;
		const window_settings m_settings;
		std::unique_ptr<user_pointer> m_user_pointer;
		bool m_has_focus;
		std::function<void(int, int)> m_framebuffer_resize_callback;
	};
}
