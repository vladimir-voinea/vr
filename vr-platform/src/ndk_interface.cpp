#include <jni.h>

#include <android/log.h>
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ndk_interface", __VA_ARGS__)

namespace vr::platform
{
    JavaVM* vm_pointer = nullptr;
    jobject activity;
    jobject asset_manager;

    JavaVM* get_java_vm()
    {
        return vm_pointer;
    }

    JNIEnv* get_jni_env()
    {
        JNIEnv* env;
        if (get_java_vm()->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            env = nullptr;
        }

        return env;
    }

    jobject get_activit()
    {
        return activity;
    }

    jobject get_asset_manager()
    {
        return asset_manager;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_set_1activity(JNIEnv * env, jclass clazz,
    jobject activity)
{
    ALOGV("Received activity");
    vr::platform::activity = env->NewGlobalRef(activity);
}

extern "C" JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_set_1asset_1manager(JNIEnv * env, jclass clazz,
    jobject manager)
{
    ALOGV("Received asset manager");
    vr::platform::asset_manager = env->NewGlobalRef(manager);
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM * vm, void* reserved) {

    ALOGV("JNI OnLoad");

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        ALOGV("JNI OnLoad error, could not get JNI version 1.6");
        return JNI_ERR;
    }

    vr::platform::vm_pointer = vm;

    return JNI_VERSION_1_6;
}