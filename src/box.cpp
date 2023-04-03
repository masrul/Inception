#include "box.hpp"
#include "real_type.hpp"
#include <string>
#include <iostream>

OBox_t::OBox_t(real_t lx, real_t ly, real_t lz): 
    m_lx{lx},
    m_ly{ly},
    m_lz{lz},
    m_nmol_types{0}
{}

void OBox_t::add(std::string file_name, size_t nmols){
    m_mol_types.emplace_back(file_name);
    m_mol_types[m_nmol_types].m_nmols = nmols; 
    ++m_nmol_types;
}

void OBox_t::pack(){

    // Find total atoms
    for (auto const& mol_type : m_mol_types){
        m_natoms += (mol_type.m_natoms * mol_type.m_nmols);
    }

    // Allocate memroy for position 
    m_pos = new real_t[m_natoms*3];
    std::cout<<"Total atoms in box: "<<m_natoms<<"\n";

    // Loop over mol_types and pack 
    for (auto const& mol_type : m_mol_types){
    

    }

    

}

