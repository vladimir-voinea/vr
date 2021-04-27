#pragma once

#include <jni.h>

namespace vr::platform
{
	JavaVM* get_java_vm();
	JNIEnv* get_jni_env();

	jobject get_activity();
	jobject get_asset_manager();
}