#pragma once

#include <shader.hpp>
#include <shader_program.hpp>

struct shaders
{
	vr::gl::shader vertex;
	vr::gl::shader fragment;
	vr::gl::shader_program program;
};
