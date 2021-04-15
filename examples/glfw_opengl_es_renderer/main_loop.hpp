#pragma once

#include "shaders.hpp"
#include "user_controls.hpp"

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>

#include <vr-gl.hpp>
#include <vr.hpp>

#include <memory>
#include <random>

class main_loop
{
public:
	main_loop(vr::glfw::window& window);
	~main_loop();

	void init();
	void run();

private:
	vr::mesh import_model(const std::string& name);
	void initialize_controls();
	void initialize_position();

	void process_input();
	void print_state();
	void render_scene();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	vr::glfw::mouse m_mouse;

	std::unique_ptr<vr::camera> m_camera;
	user_controls m_controls;
	
	float m_last_timestamp;
	float m_delta_time;

	struct monkey_data {
		vr::geometry geometry;
		vr::gl::opengl_shader* shader = nullptr;
		vr::texture* texture = nullptr;
	};
	monkey_data m_monkey_data;

	struct monkey_instance {
		std::unique_ptr<vr::object3d> obj;
		std::unique_ptr<std::vector<vr::gl::uniform>> uniforms;
		std::unique_ptr<vr::shader_material> material;
		std::unique_ptr<vr::mesh> mesh;
		glm::vec3 light_position;

		std::uniform_real_distribution<> x_rand;
		std::uniform_real_distribution<> y_rand;
		std::uniform_real_distribution<> z_rand;

		monkey_instance() = default;

		monkey_instance(monkey_instance&& other)
		{
			*this = std::move(other);
		}

		monkey_instance& operator=(monkey_instance&& other)
		{
			if (this != &other)
			{
				std::swap(obj, other.obj);
				std::swap(uniforms, other.uniforms);
				std::swap(material, other.material);
				std::swap(mesh, other.mesh);
				std::swap(light_position, other.light_position);
				std::swap(x_rand, other.x_rand);
				std::swap(y_rand, other.y_rand);
				std::swap(z_rand, other.z_rand);
			}
			return *this;
		}
	};

	std::default_random_engine m_random_engine;

	const size_t n_monkeys = 50;
	std::vector<monkey_instance> m_monkeys;



	vr::scene m_scene;
	vr::gl::renderer m_renderer;
};
