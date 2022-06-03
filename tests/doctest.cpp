//
// Created by Lykov on 16.05.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include <lib_test.h>

#include <doctest/doctest.h>

TEST_CASE("bin.dbg_folder_check") {
    path_helper p;
    auto qp = std::filesystem::current_path();
    CHECK(p.is_folder_in_path(qp));
    CHECK_EQ(p.paths_to_program("doctest.exe").size(), 1);
}
TEST_CASE("system_check"){
    path_helper p;
    CHECK(p.paths_to_program("g++.exe").size());
}

TEST_CASE("inner_methods_tests"){
    path_helper p;
    
}
