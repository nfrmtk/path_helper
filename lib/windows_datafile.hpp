//
// Created by nfrmtk on 01.10.22.
//
#ifdef _WIN32

#ifndef PATH_HELPER_WINDOWS_DATAFILE_HPP
#define PATH_HELPER_WINDOWS_DATAFILE_HPP
#include <Windows.h>
namespace files{
    class data_file{
        path_t file;
        HANDLE handle;
    public:
        using command_list = std::vector<std::string>;


        data_file& operator= (data_file&& other ) = delete;
        data_file(data_file&& other) = delete;
        data_file(const data_file&) = delete;
        data_file& operator=(const data_file&) = delete;


        bool write_versions_to_file(const data_file::command_list &full_paths);
        std::vector<std::wstring> read_versions_from_file(data_file& data);
        data_file(const path_t& desired_path);
        ~data_file();



    };
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

    bool data_file::write_versions_to_file( const map_iterator_t &executable) {
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

}
#endif //PATH_HELPER_WINDOWS_DATAFILE_HPP
#endif // _WIN32