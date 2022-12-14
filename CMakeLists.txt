cmake_minimum_required(VERSION 3.18)
project(path_helper)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)
enable_testing()

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
include_directories(lib thirdparty)
add_subdirectory(lib)
add_executable(path_helper main.cpp)
target_link_libraries(path_helper main_lib)


add_executable(isExecTest ./tests/is_executable_test.cpp)
target_link_libraries(isExecTest main_lib)
install(TARGETS isExecTest path_helper
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
        CONFIGURATIONS Release)
install(TARGETS isExecTest path_helper
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
        CONFIGURATIONS Debug)



