#pragma once

#include "vr-opengl.h"
#include <string>

namespace vr::gl
{
	GLuint load_dds(const std::string& path);
}