#ifndef MOLECULE_TRACKER_HPP

#define MOLECULE_TRACKER_HPP

#include "real_type.hpp"
#include <cstdio>
#include "molecule_type.hpp"

class MoleculeTracker_t {
public: 
    size_t m_mol_type_id;
    const MoleculeType_t& m_mol_type; 
    real_t* m_pos;
    /* std::unique_ptr<real_t[]>m_pos; */
    MoleculeTracker_t(size_t, const MoleculeType_t&,real_t*);
    /* MoleculeTracker_t& operator=(const MoleculeTracker_t& other) = default; */
    
};

#endif /* end of include guard MOLECULE_TRACKER_HPP */

