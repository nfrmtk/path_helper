//
// Created by Lykov on 16.05.2022.
//

#ifndef PATH_HELPER_LIB_TEST_H
#define PATH_HELPER_LIB_TEST_H
#include <filesystem>
#include <cstdlib>
#include <cassert>
#include <map>
class path_helper{
    using path = std::filesystem::path;

private:
    struct file_info{
        using iterator_vector = std::vector<std::vector<path>::iterator>;
        size_t counter = 0;
        iterator_vector paths;
        file_info& add(const std::vector<path>::iterator & other){
            counter++;
            paths.push_back(other);
            return *this;
        }
    };
    std::vector<path> path_parsed;
    std::vector<bool> is_folder_checked;
    std::map<path, file_info> files;
public:
    path_helper();
    void check_all_folders();

    std::pair<size_t, int8_t> check_specific_folder(const path & folder);

    void* /* TODO: avoid type deduction */ check_for_specific_program (const path& executable);

    bool is_folder_in_path(const path& folder);

    std::vector<path> paths_to_program(const path& program);
private:
    static bool if_executable(const path& file);
};

struct report{
};

#endif //PATH_HELPER_LIB_TEST_H
