#ifndef PARSE_HPP

#define PARSE_HPP

#include <vector>
#include "real_type.hpp"
#include <memory>


class Parser_t {
public: 
    // Attribute 
    size_t m_natoms;
    std::vector<std::string> m_symbols;
    std::vector<std::string> m_resnames;
    std::vector<size_t> m_resids; 
    std::vector<real_t> m_x;  
    std::vector<real_t> m_y;  
    std::vector<real_t> m_z;  
    std::unique_ptr<real_t[]>m_pos;
    
    // Methods 
    Parser_t(std::string);

private:

    // Methods
    void parse_pdb(const std::string&);
    void parse_gro(const std::string&);
    void parse_xyz(const std::string&);
    void parse_mol(const std::string&);
};

#endif /* end of include guard PARSE_HPP */

