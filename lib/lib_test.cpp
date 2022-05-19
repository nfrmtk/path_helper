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

std::pair<size_t, int8_t>     path_helper::check_specific_folder(const path& folder) {
    std::pair<size_t, int8_t> ans{0, 0};
    auto path_iterator = std::find(path_parsed.begin(), path_parsed.end(), folder);
    if (path_iterator != path_parsed.end()) {
        ans.second = 1;
    }else{
        ans.second = -1;
    }
    for (std::filesystem::directory_iterator it(folder);
         it != std::filesystem::directory_iterator(); ++it){
        const auto& entry = *it;
        if ( path_helper::if_executable(entry))
        {
            if (path_iterator != path_parsed.end()) {
                ++(files[entry.path().filename()].counter);
                files[entry.path().filename()].paths.push_back(path_iterator);
            }
            ++ans.first;
        }
    }
    if (path_iterator != path_parsed.end()) is_folder_checked[path_parsed.begin() - path_iterator] = true;
    return std::move(ans);
}

auto path_helper::check_for_specific_program(const path_helper::path &executable) {
    throw std::logic_error("not implemented");
    return nullptr;
}

bool path_helper::if_executable(const path_helper::path &file) {
    return file.has_extension() && file.filename().extension() == ".exe" || file.filename().extension() == ".bat";
}
