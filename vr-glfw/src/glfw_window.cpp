#include "glfw_window.hpp"
#include "glfw_initialization.hpp"
#include "glfw_user_pointer.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

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
		spdlog::error("GLFW error: {0}");
	}
}

namespace vr::glfw
{

	void glfw_window_focus_callback(GLFWwindow* window, int focused);
	void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height);

	window::window(window_settings settings)
		: m_settings(settings)
		, m_has_focus(false)
		, m_user_pointer(std::make_unique<user_pointer>())
	{
		m_user_pointer->window_ptr = this;
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

		if (m_settings.gl_context)
		{
			glfwWindowHint(GLFW_CLIENT_API, convert_opengl_api(m_settings.gl_context->api));
			glfwWindowHint(GLFW_OPENGL_PROFILE, convert_opengl_profile(m_settings.gl_context->profile));
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_settings.gl_context->context_version.major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_settings.gl_context->context_version.minor);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, m_settings.gl_context->foward_compatible ? GL_TRUE : GL_FALSE);
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

			glfwSetWindowUserPointer(m_window, m_user_pointer.get());
			glfwSetWindowFocusCallback(m_window, glfw_window_focus_callback);
			glfwSetFramebufferSizeCallback(m_window, glfw_framebuffer_resize_callback);
		}

		return created;
	}

	bool window::create()
	{
		m_window = glfwCreateWindow(m_settings.width, m_settings.height, m_settings.name.c_str(), m_settings.full_screen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

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

	void window::window_framebuffer_callback(int width, int height)
	{
		if (m_framebuffer_resize_callback)
		{
			m_framebuffer_resize_callback(width, height);
		}
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

	window_size window::get_viewport_size()
	{
		window_size size;
		glfwGetFramebufferSize(m_window, &size.width, &size.height);

		return size;
	}

	void window::set_framebuffer_resize_callback(std::function<void(int, int)> callback)
	{
		m_framebuffer_resize_callback = callback;
	}

	void window::swap_buffers()
	{
		glfwSwapBuffers(m_window);
	}

	void glfw_window_focus_callback(GLFWwindow* window, int focused)
	{
		void* window_user_ptr = glfwGetWindowUserPointer(window);
		auto window_instance = static_cast<vr::glfw::window*>(static_cast<user_pointer*>(window_user_ptr)->window_ptr);
		if (window_instance != nullptr)
		{
			const bool status = focused;
			spdlog::debug("Got focus: {0}", status);
			window_instance->window_focus_callback(status);
		}
		else
		{
			spdlog::error("User pointer is null");
		}
	}

	void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height)
	{
		void* window_user_ptr = glfwGetWindowUserPointer(window);
		auto window_instance = static_cast<vr::glfw::window*>(static_cast<user_pointer*>(window_user_ptr)->window_ptr);
		if (window_instance != nullptr)
		{
			window_instance->window_framebuffer_callback(width, height);
		}
		else
		{
			spdlog::error("User pointer is null");
		}
	}
}