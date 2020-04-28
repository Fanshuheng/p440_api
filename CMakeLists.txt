cmake_minimum_required(VERSION 3.13)
project(UWB)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        ${PROJECT_SOURCE_DIR}
)
add_executable(${PROJECT_NAME} rcm.c rcmIf.c  BIRL_UWB.cpp main.cpp)

add_executable(${PROJECT_NAME}_config rcm.c rcmIf.c rcmSampleApp.c )