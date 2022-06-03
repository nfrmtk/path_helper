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
    check_all_folders();
}


void path_helper::check_all_folders() {
    for (size_t i = 0; i < path_parsed.size(); ++i){
        if (!is_folder_checked[i]){
            check_specific_folder(path_parsed[i]);
        }
    }
}

void path_helper::check_specific_folder(const path_t& folder) {
    auto path_iterator = std::find(path_parsed.begin(), path_parsed.end(),folder);

    if (path_iterator != path_parsed.end()) {
        is_folder_checked[path_iterator - path_parsed.begin()] = true;
    }
    std::filesystem::directory_iterator it;
    try{
       it = std::filesystem::directory_iterator(folder);
    }
    catch(const std::filesystem::filesystem_error& err){}

    for (;it != std::filesystem::directory_iterator(); ++it){
        const path_t & entry = it->path().filename(); // e.g. A.cpp
        if ( if_executable(entry))
        {
            if (path_iterator != path_parsed.end())
                files[entry].push_back({path_iterator, std::nullopt});
        }
    }
}

bool path_helper::if_executable(const path_helper::path_t &file) {
    return file.has_extension() && file.extension() == ".exe" || file.extension() == ".bat";
}

//!
//! \param executable
//! \return completed vector of information about \p executable

auto path_helper::paths_to_program(const path_t & executable) -> derefenced_info_vector {

    auto node = files.find(executable.filename());
    set_versions(node);

    auto iters = node->second;
    std::vector< std::pair<path_t, version> > ans(iters.size());
    std::transform(iters.begin(), iters.end(), ans.begin(), dereference_info);
    return ans; // TODO: nothing is working
}

bool path_helper::is_folder_in_path( const path_t& folder) {
    return (std::find(path_parsed.begin(), path_parsed.end(), folder) != path_parsed.end());
}

void path_helper::set_versions(map_iterator_t &executable) {
    std::vector<std::string> unparsed_data = get_unparsed_version(executable);
    assert(unparsed_data.size() == executable->second.size());
    auto info_vector_it = executable->second.begin();
    for (const std::string& str: unparsed_data){
        auto version_v  = get_version(str);

        std::string filename = executable->first.string();
        std::string path_with_filename = info_vector_it->first->append(filename).string();
        std::string not_found_response = std::string("couldn't get version of ").append(path_with_filename);

        info_vector_it->second = version_v.has_value() ? *version_v : not_found_response;

        ++info_vector_it;
    }
}



bool path_helper::is_number(char letter) {
    return letter >= '0' && letter <= '9';
}

std::vector<std::string> path_helper::get_unparsed_version(const map_iterator_t& executable ) {
    auto node = *executable;
    auto info = node.second;


    CString cmd = "cmd.exe";
    CString action = "open";

    std::string Sparams = "/k "; // command to write all
    for (auto& pair : info ){
        Sparams.append(node.first.string() + " --version >> d.txt && echo newline >> d.txt && ");
    }
    CString params = Sparams.substr(0, Sparams.size() - 3).data();
    auto x = ShellExecute(NULL, action, cmd, params,
                          NULL, SW_HIDE);
    std::fstream fs("./d.txt", std::ios_base::in);
    std::string lines;
    std::vector<std::string> final_data;
    auto it = final_data.begin();
    while (std::getline(fs, lines)){
        it->append(lines.append("\n"));
        if (lines.find("newline") != -1){ // newline from 122 line
            ++it;
        }
    }
    return final_data;
}

auto path_helper::get_version(const std::string & unparsed_version) -> std::optional<version> {

    std::regex reg("([0-9]+\\.){1,5}([0-9]+)"); // version filter

    std::cmatch match;

    bool b = std::regex_search(unparsed_version.c_str(), match, reg);

    return b ? std::make_optional(std::string(*match.begin()))
             : std::nullopt;
    // TODO: solution is far from perfect
}
auto path_helper::dereference_info(const path_helper::info_type& info) -> std::pair<path_t, version> {
    return {*info.first, info.second.has_value() ? info.second.value() : "version not found"};
}





