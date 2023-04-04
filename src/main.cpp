#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "molecule_type.hpp"
#include "box.hpp"


int main(){
    OBox_t box{200.0,200.0,200.0,3.0};

    /* box.add("2-methyl-propanol.xyz",1000); */
    box.add("fmoc.pdb",2000);
    /* box.add("benzene.xyz",1200); */
    box.pack();
    box.write("pp.xyz");
    return 0; 
}
