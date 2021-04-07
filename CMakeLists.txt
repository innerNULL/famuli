# file: CMakeLists.txt
# date: 2021-04-05

 
cmake_minimum_required(VERSION 3.14)
project(famuli)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS " -pthread -std=c++17 -funroll-loops -O3 -march=native") 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


include(ExternalProject)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(fastText_pkg)
include(spdlog_pkg)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(DEMO ./demo/demo.cpp)
add_executable(demo ${DEMO})
target_link_libraries(demo PRIVATE ${fastText_lib})
