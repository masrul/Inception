#include <cstdlib>
#include "abort.hpp"
#include "box.hpp"
#include "timer.hpp"
#include "extern/toml11/toml.hpp"


int main(int argc, char* argv[]){
    
    // Set a timer 
    Timer timer{};

    // Get toml file 
    if (argc != 2) inception::abort("Please provide input file [.toml]!\n");
    std::string toml_file_name = argv[1];

    // Init Box with toml file and pack
    OBox_t box{toml_file_name};
    box.pack();

    // Print elapsed time 
    timer.print_elapsed_time();

    return EXIT_SUCCESS; 
}
