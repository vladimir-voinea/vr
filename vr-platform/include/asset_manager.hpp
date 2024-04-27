#pragma once

#include "asset.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace vr::platform
{
	class asset_manager
	{
	public:
		virtual ~asset_manager() = default;

		virtual asset get_asset_by_name(const std::string& name) = 0;
		virtual std::vector<uint8_t> read_file(const asset& asset) = 0;
		virtual bool exists(const asset& asset) = 0;
		virtual std::vector<asset> get_assets() = 0;
	};
}