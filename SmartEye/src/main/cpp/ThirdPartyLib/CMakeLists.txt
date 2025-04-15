cmake_minimum_required(VERSION 3.4.1)
SET(PROJECT_NAME SmartEye)

project(SmartEye_ThirdParty)

set(CMAKE_CXX_STANDARD 14)

set(SHOW_CMAKE_LOG YES)

message(status  "开始编译总的三方库， IS_COMPILED_THIRD_PARTY_LIB = ${IS_COMPILED_THIRD_PARTY_LIB}")
if (NOT IS_COMPILED_THIRD_PARTY_LIB)
    set(IS_COMPILED_THIRD_PARTY_LIB YES CACHE STRING DOCSTRING FORCE)
    message(status  "「IS_COMPILED_THIRD_PARTY_LIB」 被设置为 YES")

    ## 判断目标平台
    set(PLATFORM_ANDROID NO)
    set(PLATFORM_MAC NO)
    set(PLATFORM_WIN NO)
    message(status  "operation system is ${CMAKE_SYSTEM}, CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}")
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        message(status  "current platform: Windows")
        set(PLATFORM_WIN YES)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
        message(status  "current platform: mac")
        set(PLATFORM_MAC YES)
        set(CMAKE_ANDROID_ARCH_ABI x86_64)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
        message(status  "current platform: Android")
        set(PLATFORM_ANDROID YES)
    else ()
        message(status  "other platform: ${CMAKE_SYSTEM_NAME}")
    endif()
    message(status  "PLATFORM_ANDROID:${PLATFORM_ANDROID}")
    message(status  "PLATFORM_MAC:${PLATFORM_MAC}")
    message(status  "PLATFORM_WIN:${PLATFORM_WIN}")

    set(_INCLUDE_HPSocket_ NO CACHE STRING DOCSTRING FORCE)
    set(_INCLUDE_JPEG_TURBO NO CACHE STRING DOCSTRING FORCE)


    ADD_DEFINITIONS(-DHAVE_POSIX_FILEMAP)
    ADD_DEFINITIONS(-DUNIX)
    ADD_DEFINITIONS(-DCONFIG_STATICS_REPORT)
    ADD_DEFINITIONS(-DHAVE_CONFIG_H)
    ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
    ADD_DEFINITIONS(-DHAVE_NEON=1)
    ADD_DEFINITIONS(-DANDROID)

#    ADD_SUBDIRECTORY(./Wgs2gcj)
    message(status  "添加HPSocket")

    ADD_SUBDIRECTORY(./HPSocket)
    ADD_SUBDIRECTORY(./libjpeg-turbo)

    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -Winconsistent-missing-override")

    if (PLATFORM_ANDROID)
        set(ALL_FILES "")
        list(APPEND ALL_FILES Empty.cpp)
        add_library(SmartEye_ThirdParty SHARED ${ALL_FILES})
        target_link_libraries(SmartEye_ThirdParty
                -Wl,--whole-archive
#                ${WGS2GCJ_LIBRARIES}
                ${JPEG_LIBRARIES}
                HpSocket_static
                -Wl,--no-whole-archive
                )
    endif()

endif()