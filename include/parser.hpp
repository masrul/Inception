#ifndef PARSE_HPP

#define PARSE_HPP

#include <vector>
#include "real_type.hpp"


class Parser {
public: 
    Parser(std::string);
    size_t m_natoms;
    std::vector<std::string> m_symbols;
    std::vector<std::string> m_resnames;
    std::vector<size_t> m_resids; 
    std::vector<real_t> m_x;  
    std::vector<real_t> m_y;  
    std::vector<real_t> m_z;  
    void print();

private:
    void parse_pdb(const std::string&);
    void parse_gro(const std::string&);
    void parse_xyz(const std::string&);
    void parse_mol(const std::string&);
};

#endif /* end of include guard PARSE_HPP */

