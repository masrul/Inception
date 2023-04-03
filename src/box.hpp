#ifndef BOX_HPP

#define BOX_HPP

#include "real_type.hpp"
#include "molecule_type.hpp"
#include <vector>
#include <string>

class OBox_t {
public:
    // Attributes 
    real_t m_lx;
    real_t m_ly;
    real_t m_lz; 
    real_t* m_pos;
    size_t m_natoms;

    // Methods 
    OBox_t(real_t,real_t,real_t);
    void add(std::string,size_t);
    void pack();

private: 
    size_t m_nmol_types; 
    std::vector<MoleculeType_t> m_mol_types; 

};

#endif /* end of include guard BOX_HPP */

