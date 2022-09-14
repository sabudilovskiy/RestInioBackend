project(RestInoBackend)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

set(CONAN_DISABLE_CHECK_COMPILER TRUE)
include(conan_libraries/conan_paths.cmake)
include_directories(includes)
find_package(restinio REQUIRED)
find_package(magic_enum REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(taopq REQUIRED)
find_package(boost REQUIRED)

add_executable(RestInoBackend main.cpp includes/items/SystemItem src/SystemItem.cpp includes/handlers/handler_nodes_id.h includes/BDError includes/handlers/handler_imports.h includes/handlers/handler_delete_id.h includes/answers/answers.h includes/server/server.h includes/handlers/handlers_node_id_history.h includes/handlers/handler_updates.h includes/handlers/include_handlers includes/handlers/all_handlers.h includes/support/support.h)


target_link_libraries(${PROJECT_NAME} magic_enum::magic_enum)
target_link_libraries(${PROJECT_NAME} restinio::restinio)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} taocpp::taopq)
target_link_libraries(${PROJECT_NAME} Boost::boost)