#pragma once

#include "asset.hpp"

#include <string>
#include <vector>

namespace vr::platform
{
	class asset_manager
	{
	public:
		virtual asset get_asset_by_name(const std::string& name) = 0;
		virtual std::vector<uint8_t> read_file(const asset& asset) = 0;
	};
}