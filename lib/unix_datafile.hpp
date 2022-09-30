//
// Created by nfrmtk on 29.09.22.
//
#ifndef PATH_HELPER_UNIX_DATAFILE_HPP
#define PATH_HELPER_UNIX_DATAFILE_HPP
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <wait.h>

class data_file{
    using command_list = std::vector<std::string>;
    const char* data_;

public:
    explicit data_file(const char* file_name);
    bool write_versions_to_file(const command_list& full_paths);
    std::vector<std::wstring> read_versions_from_file(data_file& data);
    data_file& operator= (data_file&& other ) = delete;
    data_file(data_file&& other) = delete;
    data_file(const data_file&) = delete;
    data_file& operator=(const data_file&) = delete;
};

data_file::data_file(const char* file_name) {
    data_ = file_name;
    std::ofstream data(file_name);
    data.close();
}

bool data_file::write_versions_to_file(const data_file::command_list &full_paths) {
    for (const auto& path: full_paths){
        pid_t pid;
        if ((pid = fork()) < 0) {
            return false;
        } else if (!pid) {
            /* этот фрагмент кода выполняется в сыновнем процессе */
            execlp(path.c_str(), path.c_str(), " --version", NULL);
            _exit(0);
        } else {
            int status;
            wait(&status);
            if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                return false;
            }
            /* а этот фрагмент выполняется в родительском процессе */
        }
    }
    return true;
}

std::vector<std::wstring> data_file::read_versions_from_file(data_file &data) {
    return std::vector<std::wstring>();
}

#endif //PATH_HELPER_UNIX_DATAFILE_HPP
