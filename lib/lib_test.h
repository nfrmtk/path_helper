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
#include <thread>

struct path_helper{
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

    path_helper();

    bool is_folder_in_path(const path_t& folder);

    std::optional<derefenced_info_vector> program_info(const path_t& program);

    static std::optional<version> get_version(const std::wstring& unparsed_version);

    void set_versions(map_iterator_t& executable );

    void check_all_folders();

    void check_specific_folder(const path_t & folder);

    std::vector<std::wstring> get_unparsed_versions(const map_iterator_t& executable);

    static bool if_executable(const path_t& file);

    static derefenced_info_type dereference_info(const info_type& info);

    path_t write_versions_to_file(const map_iterator_t & executable);

    static std::vector<std::wstring> read_versions_from_file(const path_t& versions_data);
};

#endif //PATH_HELPER_LIB_TEST_H
