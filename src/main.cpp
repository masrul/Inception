#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "parse.hpp"

int main(){

    Parser prs{"2-methyl-propanol.mol"};
    prs.print();
    return 0; 
}
