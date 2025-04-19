project(${PROJECT_NAME})
cmake_minimum_required(VERSION 3.4.1)

message("### 开始编译libwgs2gcj ###")

if (NOT _INCLUDE_WGS2GCJ_ AND PLATFORM_ANDROID)
    set(_INCLUDE_WGS2GCJ_ YES CACHE STRING DOCSTRING FORCE)

    include_directories(
            ./Export_Inc
    )

    # 本工程的源文件
    file(GLOB_RECURSE LIBWGS2GCJ_SRCS *.cpp
            )
    message("CMAKE_ANDROID_ARCH_ABI:${CMAKE_ANDROID_ARCH_ABI}")

    set(PATH_WGS2GCJ_LIBS ${CMAKE_CURRENT_LIST_DIR}/libs/arm64-v8a)
    set(WGS2GCJ_LIBRARIES
            ${PATH_WGS2GCJ_LIBS}/libJni_wgs2gcj.a
            CACHE INTERNAL DOCSTRING
            )

    # 编译成静态库
    add_library(Wgs2gcj_static STATIC ${LIBWGS2GCJ_SRCS})
    target_link_libraries(Wgs2gcj_static
            ${WGS2GCJ_LIBRARIES}
            )

    # 编译为可执行文件
#    add_executable(RunWgs2gcj main.cpp)
#    target_link_libraries(RunWgs2gcj Wgs2gcj_static)

    # 测试版本编译成动态库
    if (BUILD_SHARED)
        add_library(LibWgs2gcj_shared SHARED ${LIBWGS2GCJ_SRCS})
        target_link_libraries(LibWgs2gcj_shared
                ${WGS2GCJ_LIBRARIES}
                )
    endif ()

endif ()