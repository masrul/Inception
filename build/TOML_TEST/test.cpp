/**
 * Author      : MasrulHuda (MasrulHuda@ws127-127.CHE.MsState.Edu)
 * Host        : ws127-127.CHE.MsState.Edu
 * Created     : Wednesday Apr 12, 2023 22:11:37 CDT
 */

#include <iostream>
#include "toml.hpp"

using std::cout; 
using std::cin; 
using std::endl; 

int main()
{
    auto tbl = toml::parse_file("input.toml");
    std::cout << tbl<<"\n";
    return 0;
}

