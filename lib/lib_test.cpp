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
    catch(const std::filesystem::filesystem_error& err){ return; }

    for (;it != std::filesystem::directory_iterator(); ++it){
        const path_t & entry = it->path().filename(); // e.g. A.exe, gcc.exe, or code.cmd
        if ( if_executable(entry))
        {
            if (path_iterator != path_parsed.end())
                files[entry].push_back({path_iterator, std::nullopt});
        }
    }
}

bool path_helper::if_executable(const path_helper::path_t &file) {
    return file.has_extension() && file.extension() == ".exe" || file.extension() == ".bat" || file.extension() == ".cmd";
}


auto path_helper::program_info(const path_t & program) -> std::optional<derefenced_info_vector> {

    std::string extensions[3] = {".exe", ".bat" ,".cmd"};
    auto node = files.end();
    for (const std::string& extension: extensions){
        auto program_with_extension = path_t(program)+=extension;
        if ((node = files.find(program_with_extension)) != files.end()){
            break;
        }
    }
    // auto mutnode = node->operator=(*files.begin());
    if (node == files.end()) {
        return std::nullopt;
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

        std::string filename = executable->first.string();
        std::string not_found_response = std::string("couldn't get version of ").append(filename);

        info_vector_it->second = version_v.has_value() ? *version_v : not_found_response;

        ++info_vector_it;
    }
}

std::vector<std::wstring> path_helper::get_unparsed_versions(const map_iterator_t& executable ) {
    data_file file("data.txt");
    bool state = write_versions_to_file(file, executable);
    return read_versions_from_file(file);
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


std::vector<std::wstring> path_helper::read_versions_from_file(data_file& data) {
    std::basic_fstream<wchar_t> data_file(data.file);
    std::vector<std::wstring> resulting_data;
    std::wstring temp, single_version;
    while (std::getline(data_file, temp)){
        single_version.append(temp.append(L"\n"));
        if (temp.find(L"newline") != -1){
            resulting_data.push_back(single_version);
            single_version.clear();
        }
    }
    if (!single_version.empty())resulting_data.push_back(single_version);
    return resulting_data;
}

bool path_helper::write_versions_to_file(data_file& file, const map_iterator_t &executable) {

    auto command_strings = generate_paths(executable);
    for (auto& path : command_strings){
        path.append(" --version");
    }

    // preparations for CreateProcessA
    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    BOOL ret = FALSE;
    DWORD flags = CREATE_NO_WINDOW;

    ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );
    ZeroMemory( &si, sizeof(STARTUPINFO) );
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = file.handle;
    si.hStdOutput = file.handle;

    for (std::string& command: command_strings ){
        char* c_command = command.data();
        ret = CreateProcessA(NULL, c_command, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi );
        if ( ret )
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else{
            return false;
        }
        std::ofstream data (file.file, std::fstream::app | std::fstream::in | std::fstream::out);
        data << "\nnewline\n"; // separator
        data.close();
    }

    return true;
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


path_helper::data_file::data_file(const path_helper::path_t &desired_path) {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    handle = CreateFileW( (LPCWSTR) desired_path.wstring().c_str(),
                         FILE_APPEND_DATA,
                         FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
                         &sa,
                         OPEN_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL );
    file = desired_path;
}

path_helper::data_file::~data_file() {
    std::filesystem::remove(file);
}