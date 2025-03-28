//
// Created by ZHP on 2020/10/16.
//

#ifndef ANDROID_JAVA_METHOD_CALLER_H
#define ANDROID_JAVA_METHOD_CALLER_H

#include "DataTypeDefine.h"

/**
 * C++ 层调用 Java 层方法的工具类。
 */
class CxxCallJavaHelper {

public:

    static std::string call(const String& actionType, const String& json);

};

#endif
