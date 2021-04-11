#include "glfw_window.hpp";
#include "glfw_initialization.hpp";

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace
{
	auto convert_opengl_api(vr::glfw::opengl_context_api api) {
		auto result = GLFW_NO_API;

		switch (api)
		{
		case vr::glfw::opengl_context_api::opengl_es:
		{
			result = GLFW_OPENGL_ES_API;
			break;
		}
		case vr::glfw::opengl_context_api::opengl:
		{
			result = GLFW_OPENGL_API;
			break;
		}
		}

		return result;
	}

	auto convert_opengl_profile(vr::glfw::opengl_profile profile) {
		auto result = GLFW_OPENGL_CORE_PROFILE;

		switch (profile)
		{
		case vr::glfw::opengl_profile::core:
		{
			result = GLFW_OPENGL_CORE_PROFILE;
			break;
		}
		case vr::glfw::opengl_profile::any:
		{
			result = GLFW_OPENGL_ANY_PROFILE;
			break;
		}
		}

		return result;
	}

	void glfw_error_callback(int, const char* err)
	{
		std::cerr << "GLFW error: " << err << '\n';
	}
}

namespace vr::glfw
{
	window::window(window_settings settings)
		: m_settings(settings)
		, m_has_focus(false)
	{

	}

	window::~window()
	{
		glfwDestroyWindow(m_window);
		deinitialize_glfw();
	}

	bool window::init()
	{
		if (!initialize_glfw_once()) {
			throw std::runtime_error("GLFW initialization failed");
		}

		glfwSetErrorCallback(::glfw_error_callback);

		if (m_settings.opengl_context)
		{
			glfwWindowHint(GLFW_CLIENT_API, convert_opengl_api(m_settings.opengl_context->api));
			glfwWindowHint(GLFW_OPENGL_PROFILE, convert_opengl_profile(m_settings.opengl_context->profile));
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_settings.opengl_context->context_version.major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_settings.opengl_context->context_version.minor);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, m_settings.opengl_context->foward_compatible ? GL_TRUE : GL_FALSE);
		}
		else
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		glfwWindowHint(GLFW_RESIZABLE, m_settings.resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, m_settings.msaa_samples ? *m_settings.msaa_samples : GLFW_DONT_CARE);

		const auto created = create();

		if (created)
		{
			glfwMakeContextCurrent(m_window);
			m_has_focus = true;

			glfwSetWindowUserPointer(m_window, this);
			glfwSetWindowFocusCallback(m_window, glfw_window_focus_callback);
		}

		return created;
	}

	bool window::create()
	{
		m_window = glfwCreateWindow(m_settings.width, m_settings.height, m_settings.name.c_str(), nullptr, nullptr);

		return m_window != nullptr;
	}

	GLFWwindow* window::get_handle()
	{
		return m_window;
	}

	void window::window_focus_callback(bool status)
	{
		m_has_focus = status;
	}

	void window::request_close()
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	bool window::close_requested()
	{
		return glfwWindowShouldClose(m_window);
	}

	bool window::has_focus()
	{
		return m_has_focus;
	}

	window_size window::get_size()
	{
		window_size size;
		glfwGetWindowSize(m_window, &size.width, &size.height);

		return size;
	}

	void window::swap_buffers()
	{
		glfwSwapBuffers(m_window);
	}

	void glfw_window_focus_callback(GLFWwindow* window, int focused)
	{
		void* window_user_ptr = glfwGetWindowUserPointer(window);
		auto window_instance = static_cast<vr::glfw::window*>(window_user_ptr);
		if (window_instance != nullptr)
		{
			const bool status = focused;
			std::cout << "Focus: " << std::boolalpha << status << '\n';;
			window_instance->window_focus_callback(status);
		}
		else
		{
			std::cerr << "User pointer is null" << '\n';
		}
	}
}