#include "pc_asset_manager.hpp"

#include <fstream>
#include <iterator>
#include <filesystem>
#include <algorithm>

namespace vr::platform
{
	asset pc_asset_manager::get_asset_by_name(const std::string& name)
	{
		return "data/" + name;
	}

	std::vector<uint8_t> pc_asset_manager::read_file(const asset& asset)
	{
		const auto path = asset.get_path();

		auto stream = std::ifstream(path, std::ios::binary);
		if (!stream.good())
		{
			throw std::runtime_error("Could not open asset with path " + path);
		}

		return std::vector<uint8_t> ( std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() );
	}

	bool pc_asset_manager::exists(const asset& asset)
	{
		return std::filesystem::exists(asset.get_path());
	}

	std::vector<asset> pc_asset_manager::get_assets()
	{
		std::vector<asset> result;
		std::transform(std::filesystem::recursive_directory_iterator("data/"), std::filesystem::recursive_directory_iterator(), 
			std::back_inserter(result), [](const auto& path) { return asset{ path.path().string() }; });
		
		return result;
	}
}