//
// Created by Lykov on 16.05.2022.
//

#include "lib_test.h"


path_helper::path_helper() {
    std::string PATH = std::getenv("PATH");
    const char delimiter = ';';
    size_t delim_pos = PATH.find(delimiter);
    while(delim_pos){
        path_parsed.emplace_back(PATH.substr(0, delim_pos));
        PATH.erase(0, delim_pos + 1);
        delim_pos = PATH.find(delimiter);
    }
    // in case of PATH ending with ;
    if (PATH.size())path_parsed.emplace_back(PATH);
}


auto path_helper::check_all_folders() {
    throw std::logic_error("not implemented");

}

auto path_helper::check_specific_folder(const path_helper::path &folder) {
    throw std::logic_error("not implemented");
    return nullptr;
}

auto path_helper::check_for_specific_program(const path_helper::path &executable) {
    throw std::logic_error("not implemented");
    return nullptr;
}
