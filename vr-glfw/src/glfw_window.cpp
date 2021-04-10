#include "glfw_window.hpp";
#include "glfw_initialization.hpp";

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace
{
	auto convert_opengl_api(vr::opengl_context_api api) {
		auto result = GLFW_NO_API;

		switch (api)
		{
		case vr::opengl_context_api::opengl_es:
		{
			result = GLFW_OPENGL_ES_API;
			break;
		}
		case vr::opengl_context_api::opengl:
		{
			result = GLFW_OPENGL_API;
			break;
		}
		}

		return result;
	}

	auto convert_opengl_profile(vr::opengl_profile profile) {
		auto result = GLFW_OPENGL_CORE_PROFILE;

		switch (profile)
		{
		case vr::opengl_profile::core:
		{
			result = GLFW_OPENGL_CORE_PROFILE;
			break;
		}
		case vr::opengl_profile::any:
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

namespace vr {
	glfw_window::glfw_window(glfw_window_settings settings)
		: m_settings(settings)
	{

	}

	glfw_window::~glfw_window()
	{
		glfwDestroyWindow(m_window);
		deinitialize_glfw();
	}

	bool glfw_window::init()
	{
		if (!initialize_glfw_once()) {
			throw std::runtime_error("GLFW initialization failed");
		}

		glfwSetErrorCallback(glfw_error_callback);

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

		const bool resizable = m_settings.resizable ? GLFW_TRUE : GLFW_FALSE;
		glfwWindowHint(GLFW_RESIZABLE, resizable);

		const auto created = create();

		if (created) 
		{
			glfwMakeContextCurrent(m_window);
		}

		return created;
	}

	bool glfw_window::run(i_window_loop* loop)
	{
		if (!m_window)
		{
			return false;
		}

		while (!close_requested())
		{
			glfwPollEvents();
			if (loop)
			{
				const auto continue_looping = loop->loop();
				if (!continue_looping)
				{
					return true;
				}
			}
		}

		return true;
	}

	bool glfw_window::create()
	{
		m_window = glfwCreateWindow(m_settings.width, m_settings.height, m_settings.name.c_str(), nullptr, nullptr);

		return m_window != nullptr;
	}

	bool glfw_window::close_requested()
	{
		return glfwWindowShouldClose(m_window);
	}

	void glfw_window::set_sticky_keys(bool status)
	{
		glfwSetInputMode(m_window, GLFW_STICKY_KEYS, status ? GL_TRUE : GL_FALSE);
	}
}