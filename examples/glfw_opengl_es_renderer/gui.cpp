#include "gui.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <platform_manager_factory.hpp>

#include <spdlog/spdlog.h>

#include <sstream>
#include <iomanip>

constexpr auto max_translation = 5.f;
constexpr auto max_light_translation = 20.f;
constexpr auto max_scale = 2.f;

gui::gui(vr::glfw::window& window)
	: m_model_files_regex("^.*\.(dae|obj|OBJ|fbx|glb)$")
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.get_handle(), false);
	ImGui_ImplOpenGL3_Init("#version 300 es");
}

gui::~gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void gui::frame(float delta_time)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	render_model_options(delta_time);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui::render_model_options(float delta_time)
{
	std::ostringstream s;
	s << "Time per frame: " << std::setprecision(2) << delta_time * 1000 << "ms";
	ImGui::Text(s.str().c_str());

	if (ImGui::BeginCombo("Model##m", model_parameters.path.c_str()))
	{
		const auto all_assets = vr::platform::get_platform_manager()->get_asset_manager()->get_assets();
		std::vector<std::string> model_assets;
		for (const auto& asset : all_assets)
		{
			if (std::regex_match(asset.get_path(), m_model_files_regex))
			{
				model_assets.push_back(asset.get_path());
			}
		}

		for (auto i = 0u; i < model_assets.size(); ++i)
		{
			bool is_selected = model_parameters.path == model_assets[i];
			if (ImGui::Selectable(model_assets[i].c_str(), is_selected))
			{
				model_parameters.path = model_assets[i];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::BeginGroup();
	ImGui::Text("Translation");
	ImGui::SliderFloat("X##t", &model_parameters.translation.vec.x, -max_translation, max_translation);
	ImGui::SliderFloat("Y##t", &model_parameters.translation.vec.y, -max_translation, max_translation);
	ImGui::SliderFloat("Z##t", &model_parameters.translation.vec.z, -max_translation, max_translation);
	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::Text("Rotation");
	ImGui::SliderFloat("X##r", &model_parameters.rotation.euler_angles.x, 0.f, 360.f);
	ImGui::SliderFloat("Y##r", &model_parameters.rotation.euler_angles.y, 0.f, 360.f);
	ImGui::SliderFloat("Z##r", &model_parameters.rotation.euler_angles.z, 0.f, 360.f);
	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::Text("Scale");
	ImGui::Checkbox("Uniform##s", &m_scale_uniformly);
	if (m_scale_uniformly)
	{
		ImGui::SliderFloat("Value##s", &model_parameters.scale.value.x, 0.0f, max_scale);
		model_parameters.scale.value.y = model_parameters.scale.value.x;
		model_parameters.scale.value.z = model_parameters.scale.value.x;
	}
	else
	{
		ImGui::SliderFloat("X##s", &model_parameters.scale.value.x, 0.0f, max_scale);
		ImGui::SliderFloat("Y##s", &model_parameters.scale.value.y, 0.0f, max_scale);
		ImGui::SliderFloat("Z##s", &model_parameters.scale.value.z, 0.0f, max_scale);
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::ColorEdit3("Light ambient##l", glm::value_ptr(model_parameters.light.ambient), ImGuiColorEditFlags_DisplayRGB);
	ImGui::ColorEdit3("Light diffuse##l", glm::value_ptr(model_parameters.light.diffuse), ImGuiColorEditFlags_DisplayRGB);
	ImGui::ColorEdit3("Light specular##l", glm::value_ptr(model_parameters.light.specular), ImGuiColorEditFlags_DisplayRGB);

	ImGui::Text("Translation");
	ImGui::SliderFloat("X##l", &model_parameters.light.position.x, -max_light_translation, max_light_translation);
	ImGui::SliderFloat("Y##l", &model_parameters.light.position.y, -max_light_translation, max_light_translation);
	ImGui::SliderFloat("Z##l", &model_parameters.light.position.z, -max_light_translation, max_light_translation);

	ImGui::EndGroup();


	if (ImGui::Button("Reset"))
	{
		model_parameters = {};
	}
}