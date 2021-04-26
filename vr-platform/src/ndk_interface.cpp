#include <jni.h>

#include <android/log.h>
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ndk_interface", __VA_ARGS__)

namespace vr::platform
{
    JavaVM* vm_pointer = nullptr;
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

    void set_asset_manager(JNIEnv* env, jobject object, jobject manager)
    {
        ALOGV("Received asset manager");
        asset_manager = manager;
    }
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    ALOGV("JNI OnLoad");

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        ALOGV("JNI OnLoad error, could not get JNI version 1.6");
        return JNI_ERR;
    }

    vr::platform::vm_pointer = vm;

    jclass c = env->FindClass("com/android/gles3jni/GLES3JNILib");
    if (c == nullptr)
    {
        return JNI_ERR;
    }

    static const JNINativeMethod methods[] = {
        {"set_asset_manager", "(Landroid.content.res.AssetManager)V", reinterpret_cast<void*>(vr::platform::set_asset_manager)}
    };

    int rc = env->RegisterNatives(c, methods, sizeof(methods) / sizeof(JNINativeMethod));
    if (rc != JNI_OK)
    {
        return rc;
    }

    return JNI_VERSION_1_6;
}