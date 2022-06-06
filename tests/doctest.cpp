//
// Created by Lykov on 16.05.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include <lib_test.h>

#include <doctest/doctest.h>


TEST_CASE("file read test")
{
    auto data = path_helper::read_versions_from_file("../example_data.txt");
    CHECK_EQ(data.size(), 2);
}

TEST_CASE("file write test"){
    path_helper ph;
    auto node_cxx = ph.files.find("g++.exe");
    if (node_cxx == ph.files.end()){
        return;
    }

    auto data = ph.get_unparsed_versions(node_cxx);
    CHECK_EQ(data.size(), 1);


}