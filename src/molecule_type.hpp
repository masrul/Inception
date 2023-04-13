#ifndef MOLECULE_TYPE_HPP

#define MOLECULE_TYPE_HPP
#include "real_type.hpp"
#include <vector>
#include <string>
#include <cstdio>
#include <memory>

class MoleculeType_t {
public: 
    // Attributes
    std::vector<std::string> m_symbols;
    std::vector<std::string> m_resnames;
    std::vector<size_t> m_resids; 
    std::shared_ptr<real_t[]>m_pos;
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
    bool m_is_pinned; 

    // Methods
    MoleculeType_t(std::string,size_t);
    MoleculeType_t& operator=(const MoleculeType_t&) = delete;
    void get_rand_struct(); 
    void get_rand_location(real_t&,real_t&,real_t&); 
    void translate_to(real_t,real_t,real_t);
    void copy_to(real_t*);

private: 
    size_t m_nrand_structs;
    std::shared_ptr<real_t[]>m_all_pos;
    void create_rand_structs(const std::unique_ptr<real_t[]>&);
    void find_center_atom(const std::unique_ptr<real_t[]>&);
};


#endif /* end of include guard MOLECULE_TYPE_HPP */

