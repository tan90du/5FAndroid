cmake_minimum_required(VERSION 3.4.1)

project("smarteye")

set(_INCLUDE_COMMUNICATION_ NO CACHE STRING DOCSTRING FORCE)
set(_INCLUDE_HPSocket_ NO CACHE STRING DOCSTRING FORCE)

include_directories(${HEADER_DIRECTORY}
        ./headers
        ./headers/interface
        ./headers/receiver
        ./entity
        ./Communication/headers
        ./ThirdPartyLib/HPSocket/include
        ./ThirdPartyLib/HPSocket/src
        ./ThirdPartyLib/HPSocket/src/common
        ./ThirdPartyLib/HPSocket/src/common/crypto
        ./ThirdPartyLib/HPSocket/src/common/http
        ./ThirdPartyLib/HPSocket/src/common/kcp
        ./ThirdPartyLib/Common/Export_Inc
        ./BaseLayer/JpegWrapper/Export_Inc
        ./ThirdPartyLib/Wgs2gcj/Export_Inc

)

ADD_SUBDIRECTORY(./ThirdPartyLib)
ADD_SUBDIRECTORY(./Communication)
add_subdirectory(./BaseLayer/JpegWrapper JpegWrapper.out)

#编译三方库

file(GLOB_RECURSE SRCS sources/*.cpp
        headers/interface/*.cpp
)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(status "libpath:${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        JNISmartEye.cpp
        ${SRCS}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        -Wl,--whole-archive
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libhdclient.so
        Communication_static
#        SmartEye_ThirdParty
        JpegWrapper_static
        libSmartEye_ThirdParty.so
        android
        log
        -Wl,--no-whole-archive
        -Wl,--allow-multiple-definition
)