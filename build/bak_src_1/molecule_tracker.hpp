#ifndef MOLECULE_TRACKER_HPP

#define MOLECULE_TRACKER_HPP

#include "real_type.hpp"
#include <cstdio>

class MoleculeTracker_t {
public: 
    size_t m_type_idx; 
    real_t* m_pos;

    MoleculeTracker_t(size_t,real_t*);
    
};

#endif /* end of include guard MOLECULE_TRACKER_HPP */

