#ifndef MOLECULE_TRACKER_HPP

#define MOLECULE_TRACKER_HPP

#include "real_type.hpp"
#include <cstdio>
#include "molecule_type.hpp"

class MoleculeTracker_t {
public: 
    const MoleculeType_t& m_mol_type; 
    real_t* m_pos;

    MoleculeTracker_t(const MoleculeType_t&,real_t*);
    
};

#endif /* end of include guard MOLECULE_TRACKER_HPP */

