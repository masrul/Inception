#include "molecule_tracker.hpp"

MoleculeTracker_t::MoleculeTracker_t(size_t type_idx,real_t* pos):
    m_type_idx{type_idx},
    m_pos{pos}
{}
