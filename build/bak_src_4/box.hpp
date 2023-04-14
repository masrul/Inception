#ifndef BOX_HPP

#define BOX_HPP

#include "real_type.hpp"
#include "molecule_type.hpp"
#include "molecule_tracker.hpp"
#include <vector>
#include <string>
#include <memory>

class OBox_t {
public:
    // Attributes 
    real_t m_lx;
    real_t m_ly;
    real_t m_lz; 
    real_t* m_pos;
    /* std::shared_ptr<real_t[]>m_pos; */
    size_t m_natoms;
    real_t m_rcut; 
    size_t m_max_trails;

    // Methods 
    OBox_t(real_t,real_t,real_t,real_t);
    void add(std::string, size_t);
    void add(std::string, size_t,real_t*);
    void pin(std::string, real_t*);
    std::vector<MoleculeTracker_t> m_mol_trackers; 
    void pack();
    void write(std::string);

private: 
    // Attributes 
    size_t m_nmol_types; 
    std::vector<MoleculeType_t> m_mol_types; 
    real_t m_hlx;
    real_t m_hly;
    real_t m_hlz;
    real_t m_rcut2;

    // Methods 
    bool check_overlap(MoleculeType_t &);
    void apply_pbc(real_t&, real_t&,real_t&);
    void sanity_check(); // For debug

};

#endif /* end of include guard BOX_HPP */

