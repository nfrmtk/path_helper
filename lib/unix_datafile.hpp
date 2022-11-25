//
// Created by nfrmtk on 29.09.22.
//
#if defined(__linux) || defined(__APPLE__)

#ifndef PATH_HELPER_UNIX_DATAFILE_HPP
#define PATH_HELPER_UNIX_DATAFILE_HPP
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


#ifdef __linux
#include <wait.h>
#else // __APPLE__
#include <sys/wait.h>
#endif
///asd
namespace util {
    class data_file {
        const char *filename_;
        std::ofstream& logger;
    public:
        using command_list = std::vector<std::string>;

        data_file &operator=(data_file &&other) = delete;

        data_file(data_file &&other) = delete;

        data_file(const data_file &) = delete;

        data_file &operator=(const data_file &) = delete;

        data_file(const char *file_name, std::ofstream &logger) : logger(logger), filename_(file_name) {
            std::ofstream data(filename_);
            data.close();
        }

        static std::string remove_braces(const std::string &basicString) {
            return basicString.substr(1, basicString.size() - 2);
        }

        std::filesystem::path make_command_file(const command_list &commands) {
            logger << "entered function for making command file\n";
            remove("commands.sh");

            logger << "removed command file\n";
            std::ofstream cmds("./commands.sh");
            cmds << "#!/bin/bash\n";
            for (const auto& command : commands){
                cmds << command << " --version >> data.txt" << '\n';
//                cmds.flush();
                logger << command << " --version >> data.txt" << '\n';
                cmds << "echo newline" << " >> data.txt" << '\n';
            }
            cmds.close();
            logger << "filled command file with good stuff!\n";

            return "./commands.sh";
        }

        bool write_versions_to_file(const data_file::command_list &full_paths) {
            auto file_to_execute = make_command_file(full_paths);
            if (chmod(file_to_execute.c_str(), S_IRWXU) != 0){
                logger << "couldn't change file permissions";
                return false;
            }
            if (chmod("./data.txt", S_IRWXU) != 0){
                logger << "couldn't change file permissions";
                return false;
            }
            pid_t pid;
            logger << file_to_execute.string() << "is the thing\n";
            if ((pid = fork()) < 0) {
                logger << "couldn't create a process\n";
                return false;
            } else if (pid == 0) {
                auto str_file = file_to_execute.string();
                auto _str_file = str_file;
                char* const _argv[] = {
                        str_file.data(),
                        NULL
                };
                execv(_str_file.data(), _argv);
                _exit(0);
            } else {
                wait(NULL);  // дожидаемся завершения процесса
                //remove(file_to_execute.c_str());
            }
            return true;
        }

        [[nodiscard]] std::vector<std::wstring> read_versions_from_file() const {

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
        ~data_file() {
            remove("commands.sh");
            remove("data.txt");
        }
    };




}
#endif //PATH_HELPER_UNIX_DATAFILE_HPP
#endif // __linux