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
#include <unordered_map>
#include <windows.h>


class path_helper{
private:
    template <typename T>
    using vector_iterator_t = typename std::vector<T>::iterator;

    friend void _DOCTEST_ANON_FUNC_12(); // for testing purposes
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
    using map_iterator_t = std::map<path_t, info_vector>::iterator;
public:
    path_helper();

    void check_all_folders();

    std::pair<size_t, int8_t> check_specific_folder(const path_t & folder);

    bool is_folder_in_path(const path_t& folder);

    std::vector< std::pair<path_t, version> > paths_to_program(const path_t& program);
private:
    version get_version(const std::string& unparsed_version);

    void set_versions(map_iterator_t& executable );

    std::vector<std::string> get_unparsed_version(const map_iterator_t& executable);

    static bool if_executable(const path_t& file);

    static bool is_number(char letter);
};

struct report{
};

#endif //PATH_HELPER_LIB_TEST_H
