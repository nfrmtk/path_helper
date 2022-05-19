//
// Created by Lykov on 16.05.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include <lib_test.h>


#include <doctest/doctest.h>

TEST_CASE("folder_check"){

    path_helper p;
    auto qp = std::filesystem::current_path();
    auto res = p.check_specific_folder(qp);
    CHECK(res.second == 1); // WARNING! WON'T WORK IF CMAKE WILL CHANGE
    CHECK(res.first == 2);
    CHECK(p.is_folder_in_path(qp));
    CHECK_EQ(p.paths_to_program("doctest.exe").size(), 1);

}


