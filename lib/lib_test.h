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
        uint32_t counter = 0;
        std::vector<path>::iterator paths[];
    };
    std::vector<path> path_parsed;
    std::vector<bool> is_folder_checked;
    std::map<path, file_info> files;
public:
    path_helper();
    auto /* TODO: avoid type deduction */ check_all_folders();

    uint32_t check_specific_folder(const std::vector<path>::iterator & folder);

    auto /* TODO: avoid type deduction */ check_for_specific_program (const path& executable);
private:
    static bool if_executable(const path& file);
};

struct report{

};

#endif //PATH_HELPER_LIB_TEST_H
