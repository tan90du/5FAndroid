project(${PROJECT_NAME})
cmake_minimum_required(VERSION 3.4.1)

if (NOT _INCLUDE_JPEG_TURBO AND PLATFORM_ANDROID)
    message("### 开始编译libjpeg-turbo ###")
    message("_INCLUDE_JPEG_: ${_INCLUDE_JPEG_TURBO}")
    set(_INCLUDE_JPEG_TURBO YES CACHE STRING DOCSTRING FORCE)

    include_directories(
            ./Export_Inc
    )

    # 本工程的源文件
    file(GLOB_RECURSE LIBJPEG_TURBO_SRCS Source/*.cpp
            )
    message("CMAKE_ANDROID_ARCH_ABI:${CMAKE_ANDROID_ARCH_ABI}")

    set(PATH_JPEG_LIBS ${CMAKE_CURRENT_LIST_DIR}/Source/native/staticlibs/${CMAKE_ANDROID_ARCH_ABI})
    set(JPEG_LIBRARIES
            ${PATH_JPEG_LIBS}/libturbojpeg.a
            CACHE INTERNAL DOCSTRING
            )
    message("JPEG_LIBRARIES:${JPEG_LIBRARIES}" )
#    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -shared -fPIC")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -shared -fPIC")
    # 编译成静态库


#    add_library(LibJpeg_turbo_static STATIC ${LIBJPEG_TURBO_SRCS} )
#    target_link_libraries(LibJpeg_turbo_static
#            ${JPEG_LIBRARIES}
#            )

    # 测试版本编译成动态库
    if (BUILD_SHARED)
        add_library(LibJpeg_turbo_shared SHARED ${LIBJPEG_TURBO_SRCS})
        target_link_libraries(LibJpeg_turbo_shared
                ${JPEG_LIBRARIES}
                )
    endif ()

endif ()