cmake_minimum_required(VERSION 3.13)
project(p440_api)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(p440_api
        BIRL_UWB.cpp
        BIRL_UWB.h
        CMakeLists.txt
        Command.h
        hostInterfaceCommon.h
        hostInterfaceRCM.h
        main.cpp
        usb.cpp
        usb.h)
