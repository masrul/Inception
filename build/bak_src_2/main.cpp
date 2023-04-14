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

    /* box.add("2-methyl-propanol.xyz",1000); */
    real_t bounds1[] = {0.0f,200.0f,0.0f,200.0f,0.0,100.0f};
    box.add("fmoc.pdb",1200);
    

    real_t bounds2[] = {0.0f,200.0f,0.0f,200.0f,100.0f,200.0f};
    box.add("benzene.xyz",1200,bounds2);
    
    box.pack();
    box.write("pp.xyz");

    // Print elapsed time 
    timer.print_elapsed_time();
    return 0; 
}
