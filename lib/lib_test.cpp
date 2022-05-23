//
// Created by Lykov on 16.05.2022.
//

#include "lib_test.h"


path_helper::path_helper() {
    std::string PATH = std::getenv("PATH");
    const char delimiter = ';';

    ptrdiff_t delim_pos = PATH.find(delimiter);
    while(delim_pos != -1){
        path_parsed.emplace_back(PATH.substr(0, delim_pos));
        PATH.erase(0, delim_pos + 1);
        delim_pos = PATH.find(delimiter);

    }
    path_parsed.emplace_back(PATH);

    //deleting similar consequent elements
    size_t new_size = std::distance( path_parsed.begin(), std::unique(path_parsed.begin(), path_parsed.end()));
    path_parsed.resize(new_size);
    is_folder_checked = std::vector<bool>(path_parsed.size(), false);
}


void path_helper::check_all_folders() {
    for (size_t i = 0; i < path_parsed.size(); ++i){
        if (!is_folder_checked[i]){
            auto pair = check_specific_folder(path_parsed[i]);
        }
    }
}

std::pair<size_t, int8_t> path_helper::check_specific_folder(const path& folder) {
    auto path_iterator = std::find(path_parsed.begin(), path_parsed.end(),folder);

    std::pair<size_t, int8_t> ans{0, 0};
    if (path_iterator != path_parsed.end()) {
        ans.second = 1; // stands for OK
        is_folder_checked[path_iterator - path_parsed.begin()] = true;
    }else {
        ans.second = -1; // stands for OK, BUT
    }
    std::filesystem::directory_iterator it;
    try{
       it = std::filesystem::directory_iterator(folder);
    }
    catch(const std::filesystem::filesystem_error& err){
        ans.second = -2; // stands for BAD
        ans.first = 0;
        return ans;
    }

    for (;it != std::filesystem::directory_iterator(); ++it){
        const auto& entry = it->path().filename(); // e.g. A.cpp
        if ( path_helper::if_executable(entry))
        {
            if (path_iterator != path_parsed.end())
                files[entry].push_back(path_iterator);
            ++ans.first;
        }
    }

    return ans;
}

void* path_helper::check_for_specific_program(const path_helper::path &executable) {
    throw std::logic_error("not implemented");
    return nullptr;
}

bool path_helper::if_executable(const path_helper::path &file) {
    return file.has_extension() && file.extension() == ".exe" || file.extension() == ".bat";
}

std::vector<path_helper::path> path_helper::paths_to_program(const path_helper::path & executable) {
    auto iters = files[executable];
    std::vector<std::filesystem::path> ans(iters.size());
    std::transform(iters.begin(), iters.end(), ans.begin(), [] (auto it){return *it;});
    return ans;
}

bool path_helper::is_folder_in_path( const path& folder) {
    return (std::find(path_parsed.begin(), path_parsed.end(), folder) != path_parsed.end());
}

std::string path_helper::get_version(const std::map<path, iterator_vector>::iterator& executable) {
    const LPCSTR command = "cmd.exe";
    const LPCSTR action = "open";



    const LPCSTR params =
}



