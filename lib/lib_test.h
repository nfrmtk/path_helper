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
    using path = std::filesystem::path;

private:
    template <typename T>
    using vector_iterator_t = typename std::vector<T>::iterator;

    template <typename K, typename V>
    using map_iterator_t = typename std::map<K,V>::iterator;

    using version = std::string;
    using info_type =
          std::pair<
                    vector_iterator_t<path>,
                    version
                   >;

    using info_vector =
            std::vector<info_type>;
//    struct info_vector_wrapper{
//        path_helper::info_vector data_;
//        void push_back(const auto & )
//        explicit info_vector_wrapper(const std::vector<vector_iterator_t<path>>& iters){
//            for (const auto & iter : iters ){
//                data_.emplace_back(iter, "");
//            }
//        }
//        void fill(){
//
//        }
//    };
    std::vector<path> path_parsed;
    std::vector<bool> is_folder_checked;
    std::map<path, info_vector> files;
public:
    path_helper();
    void check_all_folders();

    std::pair<size_t, int8_t> check_specific_folder(const path & folder);

    bool is_folder_in_path(const path& folder);

    auto paths_to_program(const path& program);
private:
    void get_versions(map_iterator_t<path, info_vector>& executable );
    static bool if_executable(const path& file);
};

struct report{
};

#endif //PATH_HELPER_LIB_TEST_H
