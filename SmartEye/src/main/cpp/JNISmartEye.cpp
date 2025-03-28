#include <jni.h>
#include <sys/prctl.h>
#include <sys/system_properties.h>
#include <ScopedLocalRef.h>
#include <ScopedUtfChars.h>
#include <iostream>
#include <unistd.h>
#include "Log.h"
#include "DataReceiver.h"
#include "DataReceiverHighMonitor.h"
#include "thread.h"

#ifdef __cplusplus
extern "C" {
#endif

std::string cxxCallJava(const std::string& actionType, const std::string& json);

#ifdef __cplusplus
}
#endif


/** 全局的 JavaVM 对象 */
JavaVM* javaVM = nullptr;

/** Java 层的 SmartEye 类对象 */
jclass JavaSmartEyeClassRef = nullptr;

/** Java 层的函数 */
jmethodID methodCxxCallJava = nullptr;



void _start() {

    Log::info("native-lib", "start");

    DataReceiver::start();
    Log::info("native-lib", "end");

}


std::string cxxCallJava(const std::string& actionType, const std::string& json) {
    JNIEnv* env = nullptr;
    bool needDetach = false;

    // 判断当前 C++ 的线程是否已经绑定到 JVM 的环境中了，如果没有，需要绑定
    if (javaVM->GetEnv((void**) &env, JNI_VERSION_1_4) == JNI_EDETACHED) {

        // 线程名会丢失，所以需要先保存线程名
        char threadName[32] = {0};
        prctl(PR_GET_NAME, threadName);

        // 设置绑定参数
        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_4;
        args.name = (char *)threadName;
        args.group = nullptr;

        // 绑定线程
        int attachResult = javaVM->AttachCurrentThread(&env, &args);
        if (attachResult != JNI_OK) {
            return "";
        }

        needDetach = true;
    }

    if (JavaSmartEyeClassRef == nullptr) {
        return "";
    }

    if (methodCxxCallJava == nullptr) {
        return "";
    }

    // 参数类型转换
    const char* _actionType = actionType.c_str();
    jstring __actionType = env->NewStringUTF(_actionType);

    const char* _json = json.c_str();
    jstring __json = env->NewStringUTF(_json);

    // 调用 Java 层的方法，获取返回值
    auto response = (jstring) env->CallStaticObjectMethod(JavaSmartEyeClassRef, methodCxxCallJava, __actionType, __json);

    // 将返回值做类型转换
    ScopedUtfChars _response(env, response);
    std::string __response = std::string(_response.c_str());

    // 释放掉
    env->DeleteLocalRef(__actionType);
    env->DeleteLocalRef(__json);

    return __response;
}



JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    // 保存虚拟机的引用
    javaVM = vm;

    // 获取环境
    JNIEnv *env;
    jint r = vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (r != JNI_OK) {
        return r;
    }

    // 注册 Native 方法到 JVM 中
    JNINativeMethod gMethods[] = {
            {"start", "()V", (void *) _start},
    };
    int methodCount = (sizeof(gMethods) / sizeof(gMethods[0]));
    ScopedLocalRef<jclass> uClass(env, env->FindClass("com/autonavi/smarteye/SmartEye"));
    env->RegisterNatives(uClass.get(), gMethods, methodCount);

    // 获取 Java 层的类
    JavaSmartEyeClassRef = (jclass) (env->NewGlobalRef(uClass.get()));

    // 保存 Java 的方法到 C++ 中
    methodCxxCallJava = env->GetStaticMethodID(JavaSmartEyeClassRef, "cxxCallJava", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {

}



