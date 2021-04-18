#pragma once

#include <object3d.hpp>
#include <opengl_shader_material.hpp>

#include <memory>
#include <random>

struct monkey {
	std::unique_ptr<vr::object3d> obj;
	std::unique_ptr<std::vector<vr::gl::uniform>> uniforms;
	std::unique_ptr<vr::shader_material> material;
	std::unique_ptr<vr::mesh> mesh;

	std::uniform_real_distribution<> x_rand;
	std::uniform_real_distribution<> y_rand;
	std::uniform_real_distribution<> z_rand;

	monkey() = default;

	monkey(monkey&& other)
	{
		*this = std::move(other);
	}

	monkey& operator=(monkey&& other)
	{
		if (this != &other)
		{
			std::swap(obj, other.obj);
			std::swap(uniforms, other.uniforms);
			std::swap(material, other.material);
			std::swap(mesh, other.mesh);
			std::swap(x_rand, other.x_rand);
			std::swap(y_rand, other.y_rand);
			std::swap(z_rand, other.z_rand);
		}
		return *this;
	}
};