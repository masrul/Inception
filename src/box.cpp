#include "box.hpp"
#include "real_type.hpp"
#include "molecule_tracker.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "progressbar.hpp"
#include <memory>

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
    m_max_trails = 1000;
}

void OBox_t::add(std::string file_name, size_t nmols){
    m_mol_types.emplace_back(file_name,nmols);
    
    // set bounds 
    auto& mol_type = m_mol_types[m_nmol_types];
    mol_type.m_xmin = 0;
    mol_type.m_xmax = m_lx;
    mol_type.m_ymin = 0;
    mol_type.m_ymax = m_ly;
    mol_type.m_zmin = 0;
    mol_type.m_zmax = m_lz;

    ++m_nmol_types;
}

void OBox_t::add(std::string file_name, size_t nmols,real_t* bounds){
    m_mol_types.emplace_back(file_name,nmols);
    
    // set bounds 
    auto& mol_type = m_mol_types[m_nmol_types];
    mol_type.m_xmin = bounds[0];
    mol_type.m_xmax = bounds[1];
    mol_type.m_ymin = bounds[2];
    mol_type.m_ymax = bounds[3];
    mol_type.m_zmin = bounds[4];
    mol_type.m_zmax = bounds[5];

    ++m_nmol_types;

}

void OBox_t::pack(){

    // Find total atoms
    size_t tmols=0;
    for (auto const& mol_type : m_mol_types){
        m_natoms += (mol_type.m_natoms * mol_type.m_nmols);
        std::cout<<mol_type.m_natoms<<" "<<mol_type.m_nmols<<"\n";
        tmols += mol_type.m_nmols;
    }
    std::cout<<"natoms: "<<m_natoms<<"\n";

    // Allocate memroy for position 
    m_pos.reset(new real_t[m_natoms*3]);

    // Loop over mol_types and pack 
    size_t ninserts_atom = 0; 
    srand(time(0));
    real_t posx,posy,posz;
    std::cout<<"Inception::packing\n";
    progressbar bar(tmols);
    bar.set_todo_char(" ");
    bar.set_done_char("█");
    bar.set_opening_bracket_char("[");
    bar.set_closing_bracket_char("]");
    for (auto& mol_type : m_mol_types){
        for (auto i=0u; i< mol_type.m_nmols;++i){
            size_t ntrails=0;
            bool success = false;
            while (ntrails < m_max_trails){
                // Generate random position 
                mol_type.get_rand_location(posx,posy,posz);

                // Get random struct from precalculated rotation
                mol_type.get_rand_struct(); 

                // Move to posx,posy,posz 
                mol_type.move_to(posx,posy,posz);

                // check overlap 
                auto overlap = check_overlap(mol_type); 

                if (!overlap){
                    //copy to global pos 
                    mol_type.copy_to(&m_pos[3*ninserts_atom]);
                    m_mol_trackers.emplace_back(mol_type,&m_pos[3*ninserts_atom]);
                    ninserts_atom += mol_type.m_natoms;
                    success = true;
                    bar.update();
                    break;
                }
                ++ntrails;
            }

            if (success == false){
                std::cout<< "\nCan not pack, increase num_trails or box size!\n";
                std::cout<< "Quiting ...\n";
                std::terminate();
            }
        }
    }

    std::cout<<"\n"; // celar progressbar

    // Sanity check 
    sanity_check();
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
        const auto& j_mol_type = mol_tracker.m_mol_type; 
        const auto& j_pos = mol_tracker.m_pos;
        const auto& j_rmax = j_mol_type.m_rmax; 
        const auto& j_cIDx = j_mol_type.m_cIDx;  
        const auto& j_natoms = j_mol_type.m_natoms;

        dx = i_pos[3*i_cIDx] - j_pos[3*j_cIDx];
        dy = i_pos[3*i_cIDx + 1] - j_pos[3*j_cIDx + 1];
        dz = i_pos[3*i_cIDx + 2] - j_pos[3*j_cIDx + 2];
        apply_pbc(dx,dy,dz);
        com_dist = sqrt(dx*dx + dy*dy + dz*dz); 

        if (i_rmax + j_rmax + m_rcut < com_dist) continue; 

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
    while (dx > m_hlx) dx -=m_lx;
    while (dx < -m_hlx) dx +=m_lx;

    // dy 
    while (dy > m_hly) dy -=m_ly;
    while (dy < -m_hly) dy +=m_ly;

    // dz 
    while (dz > m_hlz) dz -=m_lz;
    while (dz < -m_hlz) dz +=m_lz;
}

void OBox_t::write(std::string file_name){
    std::ofstream file{file_name};
    file<<m_natoms<<"\n"<<"\n";

    for (const auto& mol_tracker : m_mol_trackers){
        const auto& symbols = mol_tracker.m_mol_type.m_symbols; 
        const auto& natoms = mol_tracker.m_mol_type.m_natoms;
        const auto& pos = mol_tracker.m_pos;

        for (auto i=0u;i<natoms;++i){
            file
            <<symbols[i]<<" "
            <<pos[3*i] << " "
            <<pos[3*i+1] << " "
            <<pos[3*i+2] << "\n";
        }
    }
    file.close();
}

void OBox_t::sanity_check(){
    
    const auto nmols = m_mol_trackers.size(); 

    std::cout<<"\nInception::sanity_check\n";
    progressbar bar(nmols-1);
    bar.set_todo_char(" ");
    bar.set_done_char("█");
    bar.set_opening_bracket_char("[");
    bar.set_closing_bracket_char("]");

    real_t dx,dy,dz,dist2;
    real_t rmin = std::numeric_limits<real_t>::max(); 
    for (auto i = 0; i<nmols-1;++i){
        bar.update();
        const auto& i_natoms = m_mol_trackers[i].m_mol_type.m_natoms;
        const auto& i_pos = m_mol_trackers[i].m_pos;

        for (auto j = i+1; j<nmols;++j){
            const auto& j_natoms = m_mol_trackers[j].m_mol_type.m_natoms;
            const auto& j_pos = m_mol_trackers[j].m_pos;

            for (auto ii=0; ii<i_natoms; ++ii){
                for (auto jj=0u; jj<j_natoms; ++jj){
                    dx = i_pos[3*ii] - j_pos[3*jj];
                    dy = i_pos[3*ii + 1] - j_pos[3*jj + 1];
                    dz = i_pos[3*ii + 2] - j_pos[3*jj + 2];
                    apply_pbc(dx,dy,dz);
                    dist2 = dx*dx + dy*dy + dz*dz; 
                    if (dist2 < rmin) rmin = dist2;
                }
            }
        }
    }

    if (sqrt(rmin) < m_rcut){
        std::cout<<"\nSanity check failed with rmin:"<< sqrt(rmin)<<"\n";
    }
    else { 
        std::cout<<"\nSanity check passed!\n";
    }
}
