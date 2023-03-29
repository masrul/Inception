/**
 * Author      : MasrulHuda (MasrulHuda@iMacHome)
 * Host        : iMacHome
 * Created     : Wednesday Mar 29, 2023 03:43:21 CDT
 */

#include <iostream>
#include "real_type.hpp"
#include "parser.hpp"
#include "molecule_type.hpp"


MoleculeType::MoleculeType(std::string file_name) {
    Parser prs{file_name};
    prs.print();
}
