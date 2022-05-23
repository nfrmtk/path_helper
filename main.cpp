//
// Created by Lykov on 16.05.2022.
//
#include <iostream>
#include <lib_test.h>
#include <argparse/argparse.hpp>
int main(int argc, char* argv[] ){
    path_helper p;
    p.check_all_folders();
    argparse::ArgumentParser ap("path_helper", "0.1");
    ap.add_argument("").remaining();
    ap.add_description("searches in PATH for files provided");
    /*help("finds all occurences of file in user's path_t").remaining();*/
    try{
        ap.parse_args(argc, argv);
    }catch(...){
        std::cerr << "something went wrong\n";
        std::cerr << ap;
    }

    try{
        auto files = ap.get<std::vector< std::string>>("");
        for (const auto& file : files){
            auto paths = p.paths_to_program(file);
            if (paths.size()){
                std::cout << file << " is mentioned here:\n";
                for (const auto& path : paths){
                    std::cout << path.string() << "\n";
                }
            }
            else{
                std::cout << file << " is not menshioned\n";
            }
        }
    }
    catch(const std::exception& err){
        std::cout << err.what() << " no files provided\n";

    }
    std::cout << "zdes' uzhe ne pusto =)\n";
}