#pragma once

#include "glfw_window_settings.hpp"
#include "i_window_loop.hpp"

#include <string>

struct GLFWwindow;

namespace vr {
	class glfw_window {
	public:
		glfw_window(glfw_window_settings settings);
		~glfw_window();

		glfw_window(const glfw_window&) = delete;
		glfw_window& operator=(const glfw_window&) = delete;

		/**
		 * @brief Runs the window
		 *
		 * @param[in]  loop optional callback class for the window loop. can be null, in which case no callback will be invoked
		 */
		bool run(i_window_loop* loop = nullptr);

		bool create();

		bool close_requested();
	private:
		GLFWwindow* m_window = nullptr;
		const glfw_window_settings m_settings;
	};
}