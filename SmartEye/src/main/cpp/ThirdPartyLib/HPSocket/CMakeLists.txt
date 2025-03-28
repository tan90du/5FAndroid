project(${PROJECT_NAME})
cmake_minimum_required(VERSION 3.4.1)

message("### 开始编译HPSocket ###")
message("_INCLUDE_HPSocket_: ${_INCLUDE_HPSocket_}")

if (NOT _INCLUDE_HPSocket_)
set(_INCLUDE_HPSocket_ YES CACHE STRING DOCSTRING FORCE)
#ADD_DEFINITIONS(-D_BROTLI_DISABLED)
#ADD_DEFINITIONS(-D_SSL_DISABLED)
#ADD_DEFINITIONS(-D_HTTP_DISABLED)
#ADD_DEFINITIONS(-D_ICONV_DISABLED)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -Winconsistent-missing-override")

include_directories(.
        ./include
        ./src)

# 本工程的源文件
file(GLOB_RECURSE HPSOCKET_SRCS ./src/*.cpp
        ./src/common/*.cpp
        ./src/common/crypto/*.cpp
        ./src/common/http/*.cpp
        ./src/common/kcp/*.c)
# 编译成静态库
add_library(HpSocket_static STATIC ${HPSOCKET_SRCS})

endif()