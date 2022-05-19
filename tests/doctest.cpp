//
// Created by Lykov on 16.05.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include <lib_test.h>


#include <doctest/doctest.h>

TEST_CASE("folder_check"){
    path_helper p;
    auto res = p.check_specific_folder(std::filesystem::current_path());
    CHECK(res.second == -1); // WARNING! WON'T WORK IF CMAKE WILL CHANGE
    CHECK(res.first == 2);
    CHECK(true);
};


