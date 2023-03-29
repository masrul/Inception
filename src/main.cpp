#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "molecule_type.hpp"


int main(){

    /* Parser prs{"2-methyl-propanol.mol"}; */
    /* prs.print(); */
    MoleculeType mtype{"2-methyl-propanol.mol"};
    return 0; 
}
