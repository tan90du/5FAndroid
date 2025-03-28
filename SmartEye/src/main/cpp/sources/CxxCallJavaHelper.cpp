//
// Created by ZHP on 2020/10/16.
//

#include "CxxCallJavaHelper.h"

#include <utility>


extern "C" {
    std::string cxxCallJava(const std::string&  actionType, const std::string&  json);
}


std::string CxxCallJavaHelper::call(const String& actionType, const String& json) {
    return cxxCallJava(actionType, json);
}
