# file: CMakeLists.txt
# date: 2021-04-05

 
cmake_minimum_required(VERSION 3.14)
project(famuli)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS " -pthread -std=c++11 -funroll-loops -O3 -march=native") 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


# Download cmakeup as pkg management.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/main.zip")
    message(STATUS "cmakeup has been downloaded.")
else()
    execute_process(
        COMMAND wget https://ghproxy.com/https://github.com/innerNULL/cmakeup/archive/refs/heads/main.zip
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
    execute_process(
        COMMAND unzip main.zip WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
endif()

include(ExternalProject)

set(CMKAEUP_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmakeup-main/cmakeup")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMKAEUP_ROOT_PATH}/module")
include(cmakeup)
cmakeup_init("${CMAKE_CURRENT_SOURCE_DIR}/build/_cmakeup_hub" "https://ghproxy.com/https://github.com")
cmakeup_root_path_register(${CMKAEUP_ROOT_PATH})

cmakeup_pkg_cmake_importer("facebookresearch" "fastText")
cmakeup_integrate_facebookresearch_fastText("master" "null" "global")
cmakeup_pkg_cmake_importer("gabime" "spdlog")
cmakeup_integrate_gabime_spdlog("v1.x" "null" "global")
cmakeup_pkg_cmake_importer("abseil" "abseil-cpp")
cmakeup_integrate_abseil_abseil_cpp("master" "null" "global")


#aux_source_directory(./src/model/ LIB_SOURCE)

include_directories(${PROJECT_SOURCE_DIR}/include)
#add_library(famuli_static STATIC ${LIB_SOURCE})


set(DEMO ./demo/demo.cpp)
add_executable(demo ${DEMO})
#target_link_libraries(demo PUBLIC famuli_static)
target_link_libraries(demo PUBLIC ${CMAKEUP_STATIC_LIB_facebookresearch_fastText_master})
target_link_libraries(demo PUBLIC absl::strings)
