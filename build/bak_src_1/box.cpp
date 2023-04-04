#include "box.hpp"
#include "real_type.hpp"
#include "molecule_tracker.hpp"
#include <string>
#include <iostream>

OBox_t::OBox_t(real_t lx, real_t ly, real_t lz,real_t rcut): 
    m_lx{lx},
    m_ly{ly},
    m_lz{lz},
    m_rcut{rcut}
{
    m_hlx = 0.5f*lx;
    m_hly = 0.5f*ly;
    m_hlz = 0.5f*lz;
    m_rcut2 = m_rcut*m_rcut;
    m_nmol_types = 0; 
    m_natoms = 0;
}


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
    
    size_t ninserts_atom = 0; 
    size_t type_idx = 0;
    for (auto& mol_type : m_mol_types){
        size_t ninserts_mol = 0; 

        while (ninserts_mol < mol_type.m_nmols){
            // Generate random position 
            auto rand0 = ((real_t) rand() / (RAND_MAX));
            auto rand1 = ((real_t) rand() / (RAND_MAX));
            auto rand2 = ((real_t) rand() / (RAND_MAX));
            
            real_t posx = m_lx*rand0;
            real_t posy = m_lx*rand1;
            real_t posz = m_lx*rand2; 

            // Get random struct from precalculated rotation
            mol_type.get_rand_struct(); 

            // Move to posx,posy,posz 
            mol_type.move_to(posx,posy,posz);

            // check overlap 
            auto overlap = check_overlap(mol_type); 

            if (!overlap){
                //copy to global pos 
                mol_type.copy_to(&m_pos[3*ninserts_atom]);
                m_mol_trackers.emplace_back(type_idx,&m_pos[3*ninserts_atom]);
                ninserts_atom += mol_type.m_natoms;
                ++ninserts_mol;
                std::cout<<ninserts_mol <<"\n";
            }

            // Move back  to origin
            mol_type.move_to(-posx,-posy,-posz);


        }

        ++type_idx;
    }
}


bool OBox_t::check_overlap(MoleculeType_t& i_mol_type){
    
    const auto& i_pos = i_mol_type.m_pos; 
    const auto& i_rmax = i_mol_type.m_rmax; 
    const auto& i_cIDx = i_mol_type.m_cIDx;  
    const auto& i_natoms = i_mol_type.m_natoms;

    bool overlap = false; 
    real_t dx,dy,dz,com_dist,dist2;
    for (const auto& mol_tracker : m_mol_trackers){
        // Compare center-to-center 
        const auto& j_mol_type = m_mol_types[mol_tracker.m_type_idx]; 
        const auto& j_pos = mol_tracker.m_pos;
        const auto& j_rmax = j_mol_type.m_rmax; 
        const auto& j_cIDx = j_mol_type.m_cIDx;  
        const auto& j_natoms = j_mol_type.m_natoms;

        dx = i_pos[3*i_cIDx] - j_pos[3*j_cIDx];
        dy = i_pos[3*i_cIDx + 1] - j_pos[3*j_cIDx + 1];
        dz = i_pos[3*i_cIDx + 2] - j_pos[3*j_cIDx + 2];
        apply_pbc(dx,dy,dz);
        com_dist = sqrt(dx*dx + dy*dy + dz*dz); 

        if (com_dist - i_rmax - j_rmax > m_rcut) continue; 

        // Compare atom-to-atom 
        for (auto i=0; i<i_natoms; ++i){
            for (auto j=0u; j<j_natoms; ++j){
                dx = i_pos[3*i] - j_pos[3*j];
                dy = i_pos[3*i + 1] - j_pos[3*j + 1];
                dz = i_pos[3*i + 2] - j_pos[3*j + 2];
                apply_pbc(dx,dy,dz);
                dist2 = dx*dx + dy*dy + dz*dz; 

                if (dist2 < m_rcut2) return true;
            }
        }
    }

    return overlap;
}


void OBox_t::apply_pbc(real_t& dx, real_t& dy, real_t& dz){
    // dx 
    if (dx > m_hlx) dx -=m_lx;
    else if (dx < -m_hlx) dx +=m_lx;

    // dy 
    if (dy > m_hly) dy -=m_ly;
    else if (dy < -m_hly) dy +=m_ly;

    // dz 
    if (dz > m_hlz) dz -=m_lz;
    else if (dz < -m_hlz) dz +=m_lz;
}
