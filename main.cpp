//
// Created by Lykov on 16.05.2022.
//
#include <iostream>
#include <lib_test.h>
#include <argparse/argparse.hpp>

int main(int argc, char* argv[] ){
    path_helper p;
    argparse::ArgumentParser ap("path_helper", "1.0");
    ap.add_argument("file").remaining();
    ap.add_description("searches in PATH for files provided");
    /*help("finds all occurences of file in user's path_t").remaining();*/
    try{
        ap.parse_args(argc, argv);
    }catch(...){
        std::cerr << "something went wrong\n";
    }

    try{
        auto file = ap.get<std::string>("file");
        auto opt_paths = p.program_info(file);
        if (!opt_paths) {
            std::cout << "file is not found on the system\n";
            return 0;
        }
        auto paths = opt_paths.value();
        if (paths.size()){
            std::cout << file << " is mentioned here:\n";
            for (const auto& path : paths){
                std::cout << path.first.string() << "; version is -> " << path.second <<"\n";
            }
        }
        else{
            std::cout << file << " is not menshioned\n";
        }
    }
    catch(const std::exception& err){
        std::cout << "no files provided\n";
    }
}