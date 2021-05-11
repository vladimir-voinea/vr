#pragma once

#include <glfw_window.hpp>

#include <parameters.hpp>

#include <string>
#include <regex>

class gui
{
public:
	gui(vr::glfw::window& window);
	~gui();

	void frame();

private:
	void render_model_options();

public:
	parameters model_parameters;
	
private:
	bool m_scale_uniformly = true;
	std::regex m_model_files_regex;
};