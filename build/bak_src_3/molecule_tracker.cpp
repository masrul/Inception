#include "molecule_tracker.hpp"
#include "molecule_type.hpp"

MoleculeTracker_t::MoleculeTracker_t(const MoleculeType_t& mol_type,real_t* pos):
    m_mol_type{mol_type},
    m_pos{pos}
{}
