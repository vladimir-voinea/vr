#pragma once

#include "vr-opengl.h"
#include <texture.hpp>
#include <cube_texture.hpp>

#include <unordered_map>
#include <string>

namespace vr::gl
{
	GLuint load_texture(const texture* texture);
	GLuint load_texture(const cube_texture* texture);
}