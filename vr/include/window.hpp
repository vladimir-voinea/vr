#pragma once

#include "window_settings.hpp"

#include <string>

struct GLFWwindow;

namespace vr {
	class window {
	public:
		window(window_settings settings);
		~window();

		window(const window&) = delete;
		window& operator=(const window&) = delete;

		bool run();
		bool create();
		bool close_requested();
	private:
		GLFWwindow* m_window = nullptr;
		const window_settings m_settings;
	};
}