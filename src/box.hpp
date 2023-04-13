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
    size_t m_natoms;
    size_t m_nmols;
    size_t m_nmol_types; 
    real_t m_tol; 
    size_t m_max_trails;
    std::shared_ptr<real_t[]>m_pos;
    std::vector<MoleculeTracker_t> m_mol_trackers; 

    // Methods 
    OBox_t(real_t,real_t,real_t);
    OBox_t(std::string);
    void add(std::string, size_t);
    void add(std::string, size_t,real_t*);
    void pin(std::string, real_t*);
    void set_tol(real_t);
    void set_max_trails(size_t);
    void pack();
    void write(std::string);

private: 
    // Attributes 
    std::vector<MoleculeType_t> m_mol_types; 
    size_t m_rand_seed;
    real_t m_hlx;
    real_t m_hly;
    real_t m_hlz;
    real_t m_tol2;
    std::string m_out_file;
    std::vector<size_t> m_packing_order;

    // Methods 
    void init_from_toml(std::string);
    bool check_overlap(MoleculeType_t &);
    void apply_pbc(real_t&, real_t&,real_t&);
    void sanity_check(); // For debug
    void rank_packing(); // sort molecule types for packing, larger molecule first

};

#endif /* end of include guard BOX_HPP */

