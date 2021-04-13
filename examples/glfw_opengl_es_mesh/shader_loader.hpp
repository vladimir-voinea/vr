#pragma once

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

shaders load_shaders(const std::string& vertex_shader_name, const std::string& fragment_shader_name);
