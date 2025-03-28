project("bedemo2")
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 14)        # 设置 C++ 标准为 C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求使用指定的 C++ 标准

# 如果没有包含Communication模块
if (NOT _INCLUDE_COMMUNICATION_)
    set(_INCLUDE_COMMUNICATION_ NO CACHE STRING DOCSTRING FORCE)

    message(status  "正在编译 Communication")

#    ADD_DEFINITIONS(-D_BROTLI_DISABLED)
#    ADD_DEFINITIONS(-D_SSL_DISABLED)
#    ADD_DEFINITIONS(-D_HTTP_DISABLED)
#    ADD_DEFINITIONS(-D_ICONV_DISABLED)

    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -Winconsistent-missing-override")

    # 本工程引用的外部头文件目录
    include_directories(./headers
            ../ThirdPartyLib/HPSocket/include
            ../ThirdPartyLib/HPSocket/src
            ../ThirdPartyLib/HPSocket/src/common
            ../ThirdPartyLib/HPSocket/src/common/crypto
            ../ThirdPartyLib/HPSocket/src/common/http
            ../ThirdPartyLib/HPSocket/src/common/kcp
#            ../ThirdPartyLib/Wgs2gcj/Export_Inc
    )

    # 本工程的源文件
    file(GLOB_RECURSE COMMUNICATION_SRCS
            Source/*.cpp
            ./src/*.cpp
    )
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -Winconsistent-missing-override")

    # 编译成静态库
    add_library(Communication_static STATIC ${COMMUNICATION_SRCS})

    target_link_libraries(Communication_static
#            HpSocket_static
    )
endif()

