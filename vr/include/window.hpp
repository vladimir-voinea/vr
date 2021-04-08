#pragma once

#include <string>

struct GLFWwindow;

namespace vr {
	class window {
	public:
		window(int width, int height, std::string name);
		~window();

		window(const window&) = delete;
		window& operator=(const window&) = delete;

		bool run();
		bool create();
		bool close_requested();
	private:
		GLFWwindow* m_window = nullptr;
		int m_width;
		int m_height;
		std::string m_name;
	};
}