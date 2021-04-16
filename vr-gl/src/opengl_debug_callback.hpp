#pragma once

#include <vr-opengl.h>

namespace vr::gl
{
	void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
}