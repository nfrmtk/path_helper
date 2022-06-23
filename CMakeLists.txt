cmake_minimum_required(VERSION 3.22)
project(path_helper)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
include_directories(lib thirdparty)
add_subdirectory(lib)

add_executable(path_helper main.cpp)
target_link_libraries(path_helper main_lib)




