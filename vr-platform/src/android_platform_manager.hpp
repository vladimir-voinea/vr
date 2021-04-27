#pragma once

#include "platform_manager.hpp"

namespace vr::platform
{
	class android_platform_manager : public platform_manager
	{
	public:
		~android_platform_manager() override = default;

		std::unique_ptr<asset_manager> get_asset_manager() override;
	};
}