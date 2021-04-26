#include <jni.h>

namespace vr::platform
{
    JavaVM* vm_pointer;

    JavaVM* get_java_vm();
    JNIEnv* get_jni_env()
    {
        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            env = nullptr;
        }
        
        return env;
    }
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    vr::platform::vm_pointer = vm;

    //JNIEnv* env;
    //if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    //    return JNI_ERR;
    //}

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

    //return JNI_VERSION_1_6;
}