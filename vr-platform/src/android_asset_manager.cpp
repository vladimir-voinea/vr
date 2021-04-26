#include "android_asset_manager.hpp"

#include <android/asset_manager.h>

namespace vr::platform
{
	asset android_asset_manager::get_asset_by_name(const std::string& name)
	{
		return "file:///android_asset/" + name;
	}

	std::vector<uint8_t> android_asset_manager::read_file(const asset& asset)
	{
        //AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
        //AAsset* asset = AAssetManager_open(mgr, (const char*)js, AASSET_MODE_UNKNOWN);
        //if (NULL == asset) {
        //    __android_log_print(ANDROID_LOG_ERROR, NF_LOG_TAG, "_ASSET_NOT_FOUND_");
        //    return JNI_FALSE;
        //}
        //long size = AAsset_getLength(asset);
        //char* buffer = (char*)malloc(sizeof(char) * size);
        //AAsset_read(asset, buffer, size);
        //__android_log_print(ANDROID_LOG_ERROR, NF_LOG_TAG, buffer);
        //AAsset_close(asset);
        return {};
	}
}