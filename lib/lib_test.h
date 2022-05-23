//
// Created by Lykov on 16.05.2022.
//

#ifndef PATH_HELPER_LIB_TEST_H
#define PATH_HELPER_LIB_TEST_H
#include <filesystem>
#include <windows.h>
#include <string>
#include <atlstr.h>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <map>
class path_helper{
    using path = std::filesystem::path;

private:
    using iterator_vector = std::vector<std::vector<path>::iterator>;
    std::vector<path> path_parsed;
    std::vector<bool> is_folder_checked;
    std::map<path, iterator_vector> files;
public:
    path_helper();
    void check_all_folders();

    std::pair<size_t, int8_t> check_specific_folder(const path & folder);

    void* /* TODO: avoid type deduction */ check_for_specific_program (const path& executable);

    bool is_folder_in_path(const path& folder);

    std::vector<path> paths_to_program(const path& program);
private:
    static std::string get_version(const std::map<path, iterator_vector>::iterator& executable);
    static bool if_executable(const path& file);
};

struct report{
};

#endif //PATH_HELPER_LIB_TEST_H
