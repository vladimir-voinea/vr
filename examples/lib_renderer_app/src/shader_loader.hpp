#pragma once

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

shaders load_shaders(const std::string& vertex_shader_name, const std::string& fragment_shader_name);

std::string load_vertex_shader_code(const std::string& vertex_shader_name);
std::string load_fragment_shader_code(const std::string& fragment_shader_name);
