#pragma once

#include <parameters.hpp>

bool have_parameters_file(const std::string& path);
parameters load_parameters(const std::string& path);
void save_parameters(const std::string& path, const parameters& parameters);
