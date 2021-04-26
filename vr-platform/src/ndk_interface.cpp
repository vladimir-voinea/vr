#include <jni.h>

#include <android/log.h>
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ndk_interface", __VA_ARGS__)

namespace vr::platform
{
    JavaVM* vm_pointer = nullptr;

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
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    __android_log_print(ANDROID_LOG_VERBOSE, "ndk_interface", "JNI OnLoad");

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, "ndk_interface", "JNI OnLoad error, could not get JNI version 1.6");
        return JNI_ERR;
    }

    vr::platform::vm_pointer = vm;
    //// Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    //jclass c = env->FindClass("com/example/app/package/MyClass");
    //if (c == nullptr) return JNI_ERR;

    //// Register your class' native methods.
    //static const JNINativeMethod methods[] = {
    //    {"nativeFoo", "()V", reinterpret_cast<void*>(nativeFoo)},
    //    {"nativeBar", "(Ljava/lang/String;I)Z", reinterpret_cast<void*>(nativeBar)},
    //};
    //int rc = env->RegisterNatives(c, methods, sizeof(methods) / sizeof(JNINativeMethod));
    //if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}