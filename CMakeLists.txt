cmake_minimum_required(VERSION 3.22)
project(path_helper)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
include_directories(lib tests thirdparty)
add_subdirectory(lib)
add_subdirectory(tests)
add_executable(path_helper main.cpp)
target_link_libraries(path_helper lb)




