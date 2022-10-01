//
// Created by nfrmtk on 01.10.22.
//
#ifdef _WIN32

#ifndef PATH_HELPER_WINDOWS_DATAFILE_HPP
#define PATH_HELPER_WINDOWS_DATAFILE_HPP
#include <Windows.h>
namespace mythings{
    class data_file {
        const char *file_;
        HANDLE handle_;
    public:
        using command_list = std::vector<std::string>;

        ~data_file(){
            std::filesystem::remove(file_);
        };
        data_file &operator=(data_file &&other) = delete;

        data_file(data_file &&other) = delete;

        data_file(const data_file &) = delete;

        data_file &operator=(const data_file &) = delete;


        explicit data_file(const char *desired_path) {
            SECURITY_ATTRIBUTES sa;
            sa.nLength = sizeof(sa);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = TRUE;

            handle_ = CreateFileA( desired_path,
                                  FILE_APPEND_DATA,
                                 FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
                                  &sa,
                                  OPEN_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
            file_ = desired_path;
        }

        bool write_versions_to_file(command_list &&command_strings) {
            for (auto &path: command_strings) {
                path.append(" --version");
            }

            // preparations for CreateProcessA
            PROCESS_INFORMATION pi;
            STARTUPINFOA si;
            BOOL ret = FALSE;
            DWORD flags = CREATE_NO_WINDOW;

            ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            si.dwFlags |= STARTF_USESTDHANDLES;
            si.hStdInput = NULL;
            si.hStdError = handle_;
            si.hStdOutput = handle_;

            for (std::string &command: command_strings) {
                char *c_command = command.data();
                ret = CreateProcessA(NULL, c_command, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
                if (ret) {
                    WaitForSingleObject(pi.hProcess, INFINITE);
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                } else {
                    return false;
                }
                std::ofstream data(file_, std::fstream::app | std::fstream::in | std::fstream::out);
                data << "\nnewline\n"; // separator
                data.close();
            }

            return true;
        }
        std::vector<std::wstring> read_versions_from_file() const {
            std::basic_fstream<wchar_t> data_file(file_);
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
#endif //PATH_HELPER_WINDOWS_DATAFILE_HPP
#endif // _WIN32