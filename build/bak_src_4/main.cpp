#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "molecule_type.hpp"
#include "box.hpp"
#include "timer.hpp"


int main(){
    // Set a timer 
    Timer timer{};

    OBox_t box{200.0,200.0,200.0,3.0};
    real_t bounds1[] = {0.0f,200.0f,0.0f,200.0f,0.0,100.0f};
    box.add("fmoc.pdb",200,bounds1);

    real_t bounds2[] = {0.0f,200.0f,0.0f,200.0f,100.0f,200.0f};
    box.add("benzene.pdb",500,bounds2);

    /* OBox_t box{400.0,400.0,400.0,3.0}; */
    /* box.add("water.pdb",50000); */


    /* OBox_t box{100.0,100.0,100.0,3.0}; */
    /* box.add("water.pdb",5000); */
    
    box.pack();
    box.write("pp1.gro");

    // Print elapsed time 
    timer.print_elapsed_time();
    return 0; 
}
