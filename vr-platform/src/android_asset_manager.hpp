#pragma once

#include "asset_manager.hpp"

#include <android/asset_manager.h>

namespace vr::platform
{
	class android_asset_manager : public asset_manager
	{
	public:
		android_asset_manager();
		~android_asset_manager() override = default;

		asset get_asset_by_name(const std::string& name) override;
		std::vector<uint8_t> read_file(const asset& asset) override;

	private:
		AAssetManager* m_asset_manager = nullptr;
	};
}