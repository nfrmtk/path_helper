//
// Created by Lykov on 16.05.2022.
//

#ifndef PATH_HELPER_LIB_TEST_H
#define PATH_HELPER_LIB_TEST_H

#include <filesystem>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <map>
#include <unordered_map>
#include <regex>
#include <thread>
#include <optional>


#include <unix_datafile.hpp>
#include <windows_datafile.hpp>

class path_helper{
public:
    using path_t = std::filesystem::path;

    using vector_iterator_t = std::vector<path_t>::const_iterator;
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



    using map_iterator_t = std::map<std::string, info_vector>::iterator;



    using log_t = std::ofstream;

    //! environment's PATH parsing is done here, path_parsed vector and mythings map are formed;
    path_helper();

    //! for every program in info_vector version is set
    //! \param program
    //! \return empty vector
    //! \return {path,version} vector if no critical error occured
    //! \return version is unset "--version" option for corresponding path doesn't return a version
    //! \throw runtime_error if critical error during getting versions to file_ occured
    derefenced_info_vector program_info(const std::string& program);

    bool if_executable(const path_t& file);

private:
    bool is_folder_in_path(const path_t& folder);

    static std::vector<std::string> generate_paths(const map_iterator_t& executable);

    static std::optional<version> get_version(const std::wstring& unparsed_version);

    void set_versions(map_iterator_t& executable );

    void check_all_folders();

    void check_specific_folder(const path_t & folder);

    std::vector<std::wstring> get_unparsed_versions(const map_iterator_t& executable);

    static derefenced_info_type dereference_info(const info_type& info, const path_t& executable);

    void logout_map();
private:
    std::vector<path_t> path_parsed;
    log_t logger;
    std::map<std::string, info_vector> files;

};

#endif //PATH_HELPER_LIB_TEST_H
