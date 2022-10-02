//
// Created by nfrmtk on 29.09.22.
//
#ifdef __linux

#ifndef PATH_HELPER_UNIX_DATAFILE_HPP
#define PATH_HELPER_UNIX_DATAFILE_HPP
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
namespace mythings {
    class data_file {
        const char *filename_;
        std::ofstream& logger;
    public:
        using command_list = std::vector<std::string>;

        data_file &operator=(data_file &&other) = delete;

        data_file(data_file &&other) = delete;

        data_file(const data_file &) = delete;

        data_file &operator=(const data_file &) = delete;

        data_file(const char *file_name, std::ofstream &logger) : logger(logger) {
            filename_ = file_name;
            std::ofstream data(file_name);
            data.close();
        }

        static std::string remove_braces(const std::string &basicString) {
            return basicString.substr(1, basicString.size() - 2);
        }

        bool write_versions_to_file(const data_file::command_list &full_paths) {
            for (const auto &path: full_paths) {
                pid_t pid;
                auto ready_path = remove_braces(path);
                logger << ready_path + "--version\n";
                if ((pid = fork()) < 0) {
                    logger << "failed to make new process. exiting function\n";
                    return false;
                } else if (!pid) {
                    int fd = open(filename_, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

                    dup2(fd, 1);
                    close(fd);
                    execlp(ready_path.c_str(), ready_path.c_str(), "--version" ,  NULL);
                    _exit(0);
                } else {
                    int status;
                    wait(&status);
                    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                        return false;
                    } // todo: recursion. regular cycle doesn't work

                    std::ofstream data(filename_, std::fstream::app | std::fstream::in | std::fstream::out);
                    data << "\nnewline\n"; // separator
                    /* а этот фрагмент выполняется в родительском процессе */
                }
            }
            return true;
        }

        std::vector<std::wstring> read_versions_from_file() const {
            std::basic_fstream<wchar_t> data_file(filename_);
            std::vector<std::wstring> resulting_data;
            std::wstring temp, single_version;
            while (std::getline(data_file, temp)) {
                single_version.append(temp.append(L"\n"));
                if (temp.find(L"newline") != -1) {
                    resulting_data.push_back(single_version);
                    single_version.clear();
                }
            }
            if (!single_version.empty())resulting_data.push_back(single_version);
            return resulting_data;
        }
    };
}
#endif //PATH_HELPER_UNIX_DATAFILE_HPP
#endif // __linux