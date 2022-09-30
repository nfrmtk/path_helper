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


#ifdef __linux__
#include <unix_datafile.hpp>
#elif _WIN32
#include <Windows.h>
#include <atlstr.h>
#else
#endif

class path_helper{
public:
    using path_t = std::filesystem::path;
    std::vector<path_t> path_parsed;

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

    std::map<path_t, info_vector> files;

    using map_iterator_t = std::map<path_t, info_vector>::iterator;


    //! environment's PATH parsing is done here, path_parsed vector and files map are formed;
    path_helper();

    //! for every program in info_vector version is set
    //! \param program
    //! \return nullopt if program is not found
    //! \return {path,version} vector if no critical error occured
    //! \return version is unset "--version" option for corresponding path doesn't return a version
    //! \throw runtime_error if critical error during getting versions to file occured
    std::optional<derefenced_info_vector> program_info(const path_t& program);

private:
    //! raii wrapper for file creation
    struct data_file{
        path_t file;
        HANDLE handle;
        data_file(const path_t& desired_path);
        data_file& operator= (data_file&& other ) = delete;
        data_file(data_file&& other) = delete;
        data_file(const data_file&) = delete;
        data_file& operator=(const data_file&) = delete;
        ~data_file();
    };
    bool is_folder_in_path(const path_t& folder);

    static std::vector<std::string> generate_commands(const map_iterator_t& executable);

    static std::optional<version> get_version(const std::wstring& unparsed_version);

    void set_versions(map_iterator_t& executable );

    void check_all_folders();

    void check_specific_folder(const path_t & folder);

    std::vector<std::wstring> get_unparsed_versions(const map_iterator_t& executable);

    static bool if_executable(const path_t& file);

    static derefenced_info_type dereference_info(const info_type& info, const path_t& executable);

    static bool write_versions_to_file( data_file& file,  const map_iterator_t & executable);

    static std::vector<std::wstring> read_versions_from_file(data_file& data);
};

#endif //PATH_HELPER_LIB_TEST_H
