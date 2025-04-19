project(${PROJECT_NAME})
cmake_minimum_required(VERSION 3.4.1)

message("### 开始编译Jpeg封装库 ###")
message("_INCLUDE_JPEG_LIB_: ${_INCLUDE_JPEG_LIB_}")

if (NOT _INCLUDE_JPEG_LIB_)
    set(_INCLUDE_JPEG_LIB_ YES CACHE STRING DOCSTRING FORCE)

    include_directories(
            ./Export_Inc
            ../../ThirdPartyLib/libjpeg-turbo/Export_Inc
#            ../../ThirdPartyLib/Common/Export_Inc
#            ../Utils/Export_Inc/Log
#            ../../BaseLayer/Log/Export_Inc
#            ../../BaseLayer/Utils/Export_Inc
    )

    # 本工程的源文件
    file(GLOB_RECURSE JPEG_WRAPPER_SRCS ./Source/*.cpp)

    # 本工程引用的外部项目
    #    add_subdirectory(../../ThirdPartyLib/Libjpeg-turbo JpegTrubo)

    # 编译成静态库
    add_library(JpegWrapper_static STATIC ${JPEG_WRAPPER_SRCS})
    # 本工程引用的外部库
    if (HARD_ENCODE)
        target_link_libraries(JpegWrapper_static
                ${CMAKE_CURRENT_LIST_DIR}/Source/native/sharedlibs/${CMAKE_ANDROID_ARCH_ABI}/libJpegHwYUV420.so
                Business_Utils_static
                )
    endif ()

    # 测试版本编译成动态库
    if (BUILD_SHARED)
        add_library(JpegWrapper_shared SHARED ${JPEG_WRAPPER_SRCS})

        if (HARD_ENCODE)
            target_link_libraries(JpegWrapper_shared
                    ${CMAKE_CURRENT_LIST_DIR}/Source/native/sharedlibs/${CMAKE_ANDROID_ARCH_ABI}/libJpegHwYUV420.so
                    Business_Utils_static
                    )
        endif ()

    endif ()

endif ()