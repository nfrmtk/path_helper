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
    check_all_folders();
}


void path_helper::check_all_folders() {
    for (size_t i = 0; i < path_parsed.size(); ++i){
        check_specific_folder(path_parsed[i]);
    }
}

void path_helper::check_specific_folder(const path_t& folder) {
    auto path_iterator = std::find(path_parsed.begin(), path_parsed.end(),folder);

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
//! \param program
//! \return completed vector of information about \p executable

auto path_helper::program_info(const path_t & program) -> std::optional<derefenced_info_vector> {

    auto node = files.find(program.filename());
    if (node == files.end()) {
        return std::nullopt;
    }
    set_versions(node);
    auto iters = node->second;
    std::vector< std::pair<path_t, version> > ans(iters.size());
    std::transform(iters.begin(), iters.end(), ans.begin(), dereference_info);
    return ans; // TODO: nothing is working
}

void path_helper::set_versions(map_iterator_t &executable) {
    std::vector<std::string> unparsed_data = get_unparsed_versions(executable);
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

std::vector<std::string> path_helper::get_unparsed_versions(const map_iterator_t& executable ) {
    path_t data_file = write_versions_to_file(executable);
    return read_versions_from_file(data_file);
}

auto path_helper::get_version(const std::string & unparsed_version) -> std::optional<version> {

    std::regex reg("([0-9]+\\.){1,5}([0-9]+)"); // version filter

    std::cmatch match;

    bool b = std::regex_search(unparsed_version.c_str(), match, reg);

    return b ? std::make_optional(match.begin()->str())
             : std::nullopt;
    // TODO: match.begin() ???
}



auto path_helper::dereference_info(const path_helper::info_type& info) -> std::pair<path_t, version> {
    return {*info.first, info.second.has_value() ? info.second.value() : "version not found"};
}


bool path_helper::is_folder_in_path( const path_t& folder) {
    return (std::find(path_parsed.begin(), path_parsed.end(), folder) != path_parsed.end());
}



std::vector<std::string> path_helper::read_versions_from_file(const path_helper::path_t &versions_data) {
    std::ifstream data_file(versions_data);
    std::vector<std::string> resulting_data;
    std::string temp, single_version;

    while (std::getline(data_file, temp)){
        single_version.append(temp.append("\n"));
        if (temp.find("newline") != -1){
            resulting_data.push_back(single_version);
            single_version.clear();
        }
    }
    DeleteFileA(versions_data.string().c_str());
    return resulting_data;
}

path_helper::path_t path_helper::write_versions_to_file(const map_iterator_t &executable) {
    LPCSTR data_file = "data.txt";
    std::string string_params = "/k ";
    const auto node = *executable;

    for (const info_type & info: node.second){
        string_params.append((*info.first / node.first).string() + " --version >> data.txt && echo newline >> data.txt && ");
    }

    ShellExecuteA(NULL, LPCSTR("open"), LPCSTR("cmd.exe"), LPCSTR(string_params.substr(0, string_params.size() - 3).c_str()), NULL, SW_HIDE );
    std::string temp;
    std::ifstream is(data_file);
    while (!std::getline(is, temp)) {} // todo: avoid plain cycle. use something like
    return data_file;
}





