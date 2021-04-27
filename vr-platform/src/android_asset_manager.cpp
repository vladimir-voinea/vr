#include "android_asset_manager.hpp"

#include "ndk_interface.hpp"

#include <android/asset_manager_jni.h>

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace vr::platform
{
    android_asset_manager::android_asset_manager()
        : m_asset_manager(AAssetManager_fromJava(get_jni_env(), get_asset_manager()))
    {
        if (!m_asset_manager)
        {
            throw std::runtime_error("Null asset manager");
        }
    }

	asset android_asset_manager::get_asset_by_name(const std::string& name)
	{
		return name;
	}

	std::vector<uint8_t> android_asset_manager::read_file(const asset& asset)
	{
        std::vector<uint8_t> result;

        if (AAsset* a = AAssetManager_open(m_asset_manager, asset.get_path().c_str(), AASSET_MODE_UNKNOWN); a != nullptr) {
            spdlog::debug("Successfully opened asset {0}", asset.get_path());
            const auto size = AAsset_getLength(a);
            result.resize(size);
            AAsset_read(a, result.data(), result.size());
            AAsset_close(a);
        }
        else
        {
            spdlog::error("Asset {0} not found", asset.get_path());
        }

        return result;
	}
}