#include "pc_asset_manager.hpp"

#include <fstream>
#include <iterator>

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
}