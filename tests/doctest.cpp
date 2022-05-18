//
// Created by Lykov on 16.05.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <lib_test.h>


TEST_CASE("init"){
    auto ph = path_helper();
    auto a = ph.check_specific_folder("C:/asd");
    CHECK_EQ(a.first, 0);
}