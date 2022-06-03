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
#include <regex>

class path_helper{
private:
    friend void _DOCTEST_ANON_FUNC_12(); // for testing purposes
    using path_t = std::filesystem::path;
    std::vector<path_t> path_parsed;

    using vector_iterator_t = std::vector<path_t>::iterator;
    using version = std::string;
    using info_type =
          std::pair<
                    vector_iterator_t,
                    std::optional<version>
                   >;
    using derefenced_info_type = std::pair<
                                           path_t,
                                           version
                                          >;

    using info_vector =
            std::vector<info_type>;
    using derefenced_info_vector = std::vector<derefenced_info_type>;

    using map_iterator_t = std::map<path_t, info_vector>::iterator;
    std::map<path_t, info_vector> files;
public:
    path_helper();

    bool is_folder_in_path(const path_t& folder);

    derefenced_info_vector paths_to_program(const path_t& program);
private:
    static std::optional<version> get_version(const std::string& unparsed_version);

    void set_versions(map_iterator_t& executable );

    void check_all_folders();

    void check_specific_folder(const path_t & folder);

    static std::vector<std::string> get_unparsed_version(const map_iterator_t& executable);

    static bool if_executable(const path_t& file);

    static derefenced_info_type dereference_info(const info_type& info);

    static bool is_number(char letter);
};

#endif //PATH_HELPER_LIB_TEST_H
