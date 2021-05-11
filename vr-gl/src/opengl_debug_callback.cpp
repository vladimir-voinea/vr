#include "opengl_debug_callback.hpp"

#include <spdlog/spdlog.h>

namespace
{
	std::string convert_source(GLenum source)
	{
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "Window system";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "Shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "Third party";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "Application";
		case GL_DEBUG_SOURCE_OTHER:
			return "Other";
		default:
			return "Unknown source";
		}
	}

	std::string convert_type(GLenum type)
	{
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "Deprecated behavior";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "Undefined behavior";
		case GL_DEBUG_TYPE_PORTABILITY:
			return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE:
			return "Performance";
		case GL_DEBUG_TYPE_MARKER:
			return "Marker";
		case GL_DEBUG_TYPE_PUSH_GROUP:
			return "Push group";
		case GL_DEBUG_TYPE_POP_GROUP:
			return "Pop group";
		case GL_DEBUG_TYPE_OTHER:
			return "Other";
		default:
			return "Unknown type";
		}
	}

	std::string convert_severity(GLenum severity)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			return "High";
		case GL_DEBUG_SEVERITY_MEDIUM:
			return "Medium";
		case GL_DEBUG_SEVERITY_LOW:
			return "Low";
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return "Notification";
		}
	}
}

namespace vr::gl
{
	void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		// filter imgui errors
		if (id == 1282 || id == 1280)
		{
			return;
		}

		std::string msg(message);

		if (type == GL_DEBUG_TYPE_ERROR || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR || severity == GL_DEBUG_SEVERITY_HIGH)
		{
			spdlog::error("OGL: Source: {0}, Severity: {1}, Type: {2}, ID: {3}, Message: {4}", convert_source(source), convert_severity(severity), convert_type(type), id, msg);
		}
		else if (type == GL_DEBUG_TYPE_PERFORMANCE)
		{
			spdlog::warn("OGL: Source: {0}, Severity: {1}, Type: {2}, ID: {3}, Message: {4}", convert_source(source), convert_severity(severity), convert_type(type), id, msg);
		}
		else
		{
			spdlog::warn("OGL: Source: {0}, Severity: {1}, Type: {2}, ID: {3}, Message: {4}", convert_source(source), convert_severity(severity), convert_type(type), id, msg);
		}
	}
}