#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "molecule_type.hpp"
#include "box.hpp"
#include "timer.hpp"
#include "extern/toml11/toml.hpp"


int main(int argc, char* argv[]){
    
    // Set a timer 
    Timer timer{};

    /* OBox_t box{200.0,200.0,200.0,3.0}; */

    /* real_t loc[] = {100,100,100}; */
    /* box.pin("2-methyl-propanol.gro",loc); */
    
    /* real_t bounds1[] = {0.0f,200.0f,0.0f,200.0f,0.0,100.0f}; */
    /* box.add("fmoc.pdb",200,bounds1); */

    /* real_t bounds2[] = {0.0f,200.0f,0.0f,200.0f,100.0f,200.0f}; */
    /* box.add("benzene.pdb",500,bounds2); */

    /* OBox_t box{600.0,600.0,600.0,3.0}; */
    /* real_t loc[] = {200,200,200}; */
    /* box.pin("protein.pdb",loc); */
    /* box.add("water.gro",50000); */


    /* /1* OBox_t box{100.0,100.0,100.0,3.0}; *1/ */
    /* /1* box.add("water.pdb",5000); *1/ */
    
    /* box.pack(); */
    /* box.write("pp1.gro"); */

    // Get toml file 
    std::string toml_file_name = argv[1];
    OBox_t box{toml_file_name};
    box.pack();

    // Print elapsed time 
    timer.print_elapsed_time();
    return 0; 
}
