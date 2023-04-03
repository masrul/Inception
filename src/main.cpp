#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "molecule_type.hpp"
#include "box.hpp"


int main(){

    /* Parser prs{"2-methyl-propanol.mol"}; */
    /* prs.print(); */
    /* MoleculeType_t mtype{"2-methyl-propanol.xyz"}; */

    OBox_t box{100.0,100.0,100.0};

    box.add("2-methyl-propanol.xyz",100);
    box.add("benzene.xyz",100);
    box.pack();
    return 0; 
}
