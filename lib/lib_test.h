//
// Created by Lykov on 16.05.2022.
//

#ifndef PATH_HELPER_LIB_TEST_H
#define PATH_HELPER_LIB_TEST_H
#include <argparse/argparse.hpp>
#include <filesystem>
#include <cstdlib>
class path_helper{
    using path = std::filesystem::path;
private:




    struct file_info{
        uint32_t counter;
        std::optional<std::vector<path *>> paths;x`argparse
    };
    std::vector<path> path_parsed;
public:
    path_helper();
    auto /* TODO: avoid type deduction */ check_all_folders();

    auto /* TODO: avoid type deduction */ check_specific_folder(const path & folder);

    auto /* TODO: avoid type deduction */ check_for_specific_program (const path& executable);

};


#endif //PATH_HELPER_LIB_TEST_H
