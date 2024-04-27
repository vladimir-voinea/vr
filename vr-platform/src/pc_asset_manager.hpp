#pragma once

#include "asset_manager.hpp"
#include <cstdint>

namespace vr::platform
{
	class pc_asset_manager : public asset_manager
	{
	public:
		~pc_asset_manager() override = default;

		asset get_asset_by_name(const std::string& name) override;
		std::vector<uint8_t> read_file(const asset& asset) override;
		bool exists(const asset& asset) override;
		std::vector<asset> get_assets() override;
	};
}