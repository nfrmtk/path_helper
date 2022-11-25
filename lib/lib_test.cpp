//
// Created by Lykov on 16.05.2022.
//

#include "lib_test.h"


path_helper::path_helper() {
    logger = log_t{"all.log"};



    std::string PATH = std::getenv("PATH");
#ifdef _WIN32
    const char delimiter = ';';
#endif
#if defined(__linux) || defined(__APPLE__)
    const char delimiter = ':';
#endif
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
    for (const auto & folder : path_parsed){
        check_specific_folder(folder);
    }
    //logout_map();
}

void path_helper::check_specific_folder(const path_t& folder) {
    auto path_iterator = std::find(path_parsed.begin(), path_parsed.end(),folder);

    std::filesystem::directory_iterator it;
    try{
       it = std::filesystem::directory_iterator(folder);
    }
    catch(const std::filesystem::filesystem_error& err){
        return;
    }

    for (;it != std::filesystem::directory_iterator(); ++it){
        const path_t & entry = it->path();
        auto str_entry = entry.string();
        if ( if_executable(entry))
        {
            if (path_iterator != path_parsed.end()){
                files[entry.filename()].push_back({path_iterator, std::nullopt});
            }
        }
    }
}

bool path_helper::if_executable(const path_helper::path_t &file) {
#ifdef _WIN32
    return file.has_extension() && file.extension() == ".exe" || file.extension() == ".bat" || file.extension() == ".cmd";
#endif
#if defined(__linux) || defined(__APPLE__)
    return ! access (file.c_str(), X_OK);
#endif
}


auto path_helper::program_info(const std::string & program) -> derefenced_info_vector {
#ifdef _WIN32
    const std::string extensions[3] = {".exe", ".bat" ,".cmd"};
#endif
#if defined(__linux) || defined(__APPLE__)
    const std::string extensions[1] = {""};
#endif
    auto node = files.end();
    for (const std::string& extension: extensions){
        auto program_with_extension =program+extension;
        if ((node = files.find(program_with_extension)) != files.end()){
            break;
        }
    }
    // auto mutnode = node->operator=(*mythings.begin());
    if (node == files.end()) {
        return {};
    }
    set_versions(node);
    auto iters = node->second;
    const auto name = node->first;
    std::vector< std::pair<path_t, version> > ans(iters.size());
    std::transform(iters.begin(), iters.end(), ans.begin(), [ &name] (const info_type & info ) { return dereference_info(info, name); });
    return ans;
}

void path_helper::set_versions(map_iterator_t &executable) {

    std::vector<std::wstring> unparsed_data = get_unparsed_versions(executable);
    if (unparsed_data.size() != executable->second.size()) throw std::runtime_error("parsing error");
    auto info_vector_it = executable->second.begin();
    for (const std::wstring& str: unparsed_data){
        auto version_v  = get_version(str);

        std::string filename = executable->first;
        std::string not_found_response = std::string("couldn't get version of ").append(filename);

        info_vector_it->second = version_v.has_value() ? *version_v : not_found_response;

        ++info_vector_it;
    }
}

std::vector<std::wstring> path_helper::get_unparsed_versions(const map_iterator_t& executable ) {
    util::data_file file("data.txt", logger);
    if (!file.write_versions_to_file(generate_paths(executable))){
        return {};
    }
    return file.read_versions_from_file();
}

auto path_helper::get_version(const std::wstring & unparsed_version) -> std::optional<version> {

    std::wregex reg(L"([0-9]+\\.){1,5}([0-9]+)"); // version filter

    std::match_results<const wchar_t*> match;

    bool if_found_anything = std::regex_search(unparsed_version.c_str(), match, reg);
    if (!if_found_anything){
        return std::nullopt;
    }

    std::wstring ans = match.begin()->str();
    return std::make_optional(std::string(ans.begin(), ans.end()));

    // TODO: match.begin() ???
}

auto path_helper::dereference_info(const path_helper::info_type& info, const path_t& executable) -> std::pair<path_t, version> {
    return {(*info.first)/executable, info.second.has_value() ? info.second.value() : "version not found"};
}


bool path_helper::is_folder_in_path( const path_t& folder) {
    return (std::find(path_parsed.begin(), path_parsed.end(), folder) != path_parsed.end());
}

std::vector<std::string> path_helper::generate_paths(const path_helper::map_iterator_t &executable) {
    std::vector<std::string> final_vector;
    const auto node = *executable;


    for (const info_type & info: node.second){
        std::string command;
        command = ('\"' + (*info.first / node.first).string() + '\"');
        final_vector.emplace_back(command);
    }


    return final_vector;
}

void path_helper::logout_map() {
    for(const auto& node : files ){
        logger << node.first << '\n';
    }
}
