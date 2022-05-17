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
    is_folder_checked = std::vector<bool>(path_parsed.size(), false);
}


auto path_helper::check_all_folders() {
    throw std::logic_error("not implemented");

}

uint32_t path_helper::check_specific_folder(const std::vector<path>::iterator& folder) {
    std::filesystem::directory_iterator it(*folder);
    uint32_t ans = 0;
    for (; it != std::filesystem::directory_iterator(); ++it){
        auto file = *it;
        if ( path_helper::if_executable(file))
        {
            ++(files[it->path().filename()].counter);
            // files[it->path().filename()].paths[1] = folder; // TODO: not working yet
            ++ans;
        }

    }
    // is_folder_checked[path_parsed.begin() - folder] = true; // TODO: uncomment if previous todo is fixed
    return ans;
}

auto path_helper::check_for_specific_program(const path_helper::path &executable) {
    throw std::logic_error("not implemented");
    return nullptr;
}

bool path_helper::if_executable(const path_helper::path &file) {
    return file.has_extension() && file.filename().extension() == ".exe" || file.filename().extension() == ".bat";
}
