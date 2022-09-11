project(RestInoBackend)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

set(CONAN_DISABLE_CHECK_COMPILER TRUE)
include(conan_libraries/conan_paths.cmake)

find_package(restinio)

add_executable(RestInoBackend main.cpp)

target_link_libraries(${PROJECT_NAME} restinio::restinio)
