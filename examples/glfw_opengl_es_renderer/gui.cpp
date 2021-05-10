#include "gui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

gui::gui(vr::glfw::window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.get_handle(), false);
	ImGui_ImplOpenGL3_Init("#version 300 es");

	text_input.reserve(300);
}

void gui::frame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Text("Hello, world %d", 123);
	if (ImGui::Button("Save"))
	{
		spdlog::info("Save clicked!");
	}

	ImGui::InputText("Text", text_input.data(), text_input.capacity());

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}