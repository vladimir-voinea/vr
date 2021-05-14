#include "parameters_persistency.hpp"

#include <cereal/archives/json.hpp>
#include <fstream>
#include <filesystem>

bool have_parameters_file(const std::string& path)
{
	return std::filesystem::exists(path);
}

parameters load_parameters(const std::string& path)
{
	parameters result;

	std::ifstream f(path);
	cereal::JSONInputArchive ar(f);
	ar(result);

	return result;
}

void save_parameters(const std::string& path, const parameters& parameters)
{
	std::ofstream f(path);
	cereal::JSONOutputArchive ar(f);

	ar(parameters);
}
