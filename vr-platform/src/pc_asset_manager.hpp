#pragma once

#include "asset_manager.hpp"

namespace vr::platform
{
	class pc_asset_manager : public asset_manager
	{
	public:
		asset get_asset_by_name(const std::string& name) override;
		std::vector<uint8_t> read_file(const asset& asset) override;
	};
}