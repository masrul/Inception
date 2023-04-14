#ifndef MOLECULE_TYPE_HPP

#define MOLECULE_TYPE_HPP
#include "real_type.hpp"
#include <vector>
#include <string>
#include <cstdio>

class MoleculeType_t {
public: 
    MoleculeType_t(std::string);
    ~MoleculeType_t();
    std::vector<std::string> m_symbols;
    std::vector<std::string> m_resnames;
    std::vector<size_t> m_resids; 
    real_t* m_pos; 
    size_t m_natoms;
    size_t m_nmols;
    size_t m_cIDx;
    real_t m_rmax;
    real_t m_xmin; // bounds 
    real_t m_ymin;
    real_t m_zmin;
    real_t m_xmax;
    real_t m_ymax;
    real_t m_zmax;

    void get_rand_struct(); 
    void get_rand_location(real_t&,real_t&,real_t&); 
    void move_to(real_t,real_t,real_t);
    void copy_to(real_t*);

private: 
    size_t m_nrand_structs=10000;
    real_t* m_all_pos;
    void create_rand_structs(real_t*);
    void find_center_atom(real_t*);
    void write(); // For debug 
};


#endif /* end of include guard MOLECULE_TYPE_HPP */

