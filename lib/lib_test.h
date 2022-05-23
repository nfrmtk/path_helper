//
// Created by Lykov on 16.05.2022.
//

#ifndef PATH_HELPER_LIB_TEST_H
#define PATH_HELPER_LIB_TEST_H
#include <filesystem>
#include <string>
#include <atlstr.h>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <map>
#include <windows.h>
class path_helper{
private:
    template <typename T>
    using vector_iterator_t = typename std::vector<T>::iterator;

    template <typename K, typename V>
    using map_iterator_t = typename std::map<K,V>::iterator;

    using path_t = std::filesystem::path;
    using version = std::string;
    using info_type =
          std::pair<
                    vector_iterator_t<path_t>,
                    version
                   >;

    using info_vector =
            std::vector<info_type>;
    std::vector<path_t> path_parsed;
    std::vector<bool> is_folder_checked;
    std::map<path_t, info_vector> files;
public:
    path_helper();
    void check_all_folders();

    std::pair<size_t, int8_t> check_specific_folder(const path_t & folder);

    bool is_folder_in_path(const path_t& folder);

    auto paths_to_program(const path_t& program);
private:
    void get_versions(map_iterator_t<path_t, info_vector>& executable );
    static bool if_executable(const path_t& file);
};

struct report{
};

#endif //PATH_HELPER_LIB_TEST_H
