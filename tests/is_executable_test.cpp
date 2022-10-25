//
// Created by nfrmtk on 02.10.22.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <lib_test.h>

TEST_CASE("main") {

    CHECK(path_helper().if_executable(std::filesystem::current_path()));
}