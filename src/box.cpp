#include "box.hpp"
#include "real_type.hpp"
#include "molecule_tracker.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "extern/progressbar.hpp"
#include <memory>
#include <cassert>
#include "writer.hpp"
#include "abort.hpp"
#include "math_util.hpp"
#include "extern/toml11/toml.hpp"
#include "timer.hpp"

enum {RANDOM,PINNED}; // Packing type 

OBox_t::OBox_t(real_t lx, real_t ly, real_t lz): 
    m_lx{lx},
    m_ly{ly},
    m_lz{lz}
{
    m_nmol_types = 0; 
    m_natoms = 0;
    m_max_trails = 1000;

    m_hlx = 0.5f*lx;
    m_hly = 0.5f*ly;
    m_hlz = 0.5f*lz;

    m_tol2 = m_tol*m_tol;

    m_rand_seed=time(0); // not a good random number, but we don't need good randomness
    srand(m_rand_seed);
}

OBox_t::OBox_t(std::string toml_file_name){
    m_natoms = 0;
    m_nmols = 0; 
    m_nmol_types = 0; 

    srand(m_rand_seed);
    
    // Get user input from toml file
    init_from_toml(toml_file_name); 
}


void OBox_t::set_tol(real_t tol){
    m_tol = tol;
    m_tol2 = tol*tol;
}

void OBox_t::set_max_trails(size_t max_trails){
    m_max_trails = max_trails;
}

void OBox_t::add(std::string file_name, size_t nmols){

    // Add mol_type 
    m_mol_types.emplace_back(file_name,nmols);
    
    // set bounds 
    auto& mol_type = m_mol_types[m_nmol_types];
    mol_type.m_xmin = 0;
    mol_type.m_xmax = m_lx;
    mol_type.m_ymin = 0;
    mol_type.m_ymax = m_ly;
    mol_type.m_zmin = 0;
    mol_type.m_zmax = m_lz;

    // set is pinned 
    mol_type.m_is_pinned = false;

    ++m_nmol_types;
}


void OBox_t::add(std::string file_name, size_t nmols,real_t* bounds){

    // Add mol_type 
    m_mol_types.emplace_back(file_name,nmols);
    
    // set bounds 
    auto& mol_type = m_mol_types[m_nmol_types];
    mol_type.m_xmin = bounds[0];
    mol_type.m_xmax = bounds[1];
    mol_type.m_ymin = bounds[2];
    mol_type.m_ymax = bounds[3];
    mol_type.m_zmin = bounds[4];
    mol_type.m_zmax = bounds[5];

    // set is pinned 
    mol_type.m_is_pinned = false;

    ++m_nmol_types;
}

void OBox_t::pin(std::string file_name, real_t* loc){

    // Add mol_type 
    m_mol_types.emplace_back(file_name,static_cast<size_t>(1));
    
    // set bounds, for pinned, max==min 
    auto& mol_type = m_mol_types[m_nmol_types];
    mol_type.m_xmin = loc[0];
    mol_type.m_xmax = loc[0];
    mol_type.m_ymin = loc[1];
    mol_type.m_ymax = loc[1];
    mol_type.m_zmin = loc[2];
    mol_type.m_zmax = loc[2];

    // set is pinned 
    mol_type.m_is_pinned = false;

    ++m_nmol_types;
}

void OBox_t::pack(){

    // Find total atoms and mols in the box 
    for (auto const& mol_type : m_mol_types){
        m_natoms += (mol_type.m_natoms * mol_type.m_nmols);
        m_nmols += mol_type.m_nmols;
    }

    // Allocate memroy for position (shared_ptr)
    m_pos.reset(new real_t[m_natoms*3]);

    // Set progressbar for packing 
    progressbar bar(m_nmols);
    bar.set_todo_char(" ");
    bar.set_done_char("█");
    bar.set_opening_bracket_char("[");
    bar.set_closing_bracket_char("]");

    // Loop over mol_types and pack 
    size_t natoms_inserted = 0; 
    real_t posx,posy,posz;
    std::cout<<"Inception::packing\n";
    rank_packing(); // ranking such that larger molecules go first! 

    for (auto& mol_type_id : m_packing_order){
        auto& mol_type = m_mol_types[mol_type_id];
        for (auto i=0u; i< mol_type.m_nmols;++i){
            size_t ntrails=0;
            bool success = false;
            while (ntrails < m_max_trails){
                // Generate random position 
                mol_type.get_rand_location(posx,posy,posz);

                // Get random struct from precalculated rotation
                mol_type.get_rand_struct(); 
                
                // Translate molecule to posx,posy,posz 
                mol_type.translate_to(posx,posy,posz);

                // check overlap 
                auto overlap = check_overlap(mol_type); 

                // If passed, save to global memory
                if (!overlap){
                    //copy to global pos 
                    mol_type.copy_to(&m_pos[3*natoms_inserted]);
                    m_mol_trackers.emplace_back(
                        mol_type_id,
                        mol_type,
                        &m_pos[3*natoms_inserted]
                    );
                    natoms_inserted += mol_type.m_natoms;
                    success = true;
                    bar.update();
                    break;
                }
                ++ntrails;
            }

            if (!success){
                inception::abort(
                    "\nCan not pack, increase max_trails or box size!\n"
                );
            }
        }
    }

    // celaning progressbar
    std::cout<<"\n";

    // Sanity check (debugging purpose only, user does not need to check) 
    if (m_sanity) sanity_check();

    // write to file 
    write(m_out_file);
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

        if (i_rmax + j_rmax + m_tol < com_dist) continue; 

        // Compare atom-to-atom 
        for (auto i=0; i<i_natoms; ++i){
            for (auto j=0u; j<j_natoms; ++j){
                dx = i_pos[3*i] - j_pos[3*j];
                dy = i_pos[3*i + 1] - j_pos[3*j + 1];
                dz = i_pos[3*i + 2] - j_pos[3*j + 2];
                apply_pbc(dx,dy,dz);
                dist2 = dx*dx + dy*dy + dz*dz; 

                if (dist2 < m_tol2) return true;
            }
        }
    }
    return overlap;
}

void OBox_t::apply_pbc(real_t& dx, real_t& dy, real_t& dz){
/** 
 * "while" is used instead of "if"
 * As, larger molecule might be longer than half of  box length!
**/

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

void OBox_t::rank_packing(){
    std::vector<size_t> scores; 
    size_t max_natoms = std::numeric_limits<size_t>::min(); 
 
    // Score is proportional to natoms
    for (const auto& mol_type : m_mol_types){
        const auto& natoms = mol_type.m_natoms; 
        scores.push_back(natoms); 
        if (natoms > max_natoms) max_natoms = natoms;
    }
    
    // To set higher prioprity of pinned molecules, add max_natoms to them 
    for (auto i=0u; i < scores.size();++i){
        if (m_mol_types[i].m_is_pinned){
            scores[i] +=max_natoms;
        }
    }

    // sort them based on index
    m_packing_order = MathUtil::argsort(scores);
}

void OBox_t::write(std::string file_name){
    Writer_t writer{*this,file_name};
}

void OBox_t::sanity_check(){
    
    const auto nmols = m_mol_trackers.size(); 
    
    // Set progressbar for sanity check 
    progressbar bar(nmols-1);
    bar.set_todo_char(" ");
    bar.set_done_char("█");
    bar.set_opening_bracket_char("[");
    bar.set_closing_bracket_char("]");
    std::cout<<"\nInception::sanity_check\n";

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

    if (sqrt(rmin) < m_tol){
        std::cout<<"\nSanity check failed with rmin:"<< sqrt(rmin)<<"\n";
    }
    else { 
        std::cout<<"\n\nSanity check passed!\n";
    }
}



void OBox_t::init_from_toml(std::string toml_file_name){

    const auto input = toml::parse(toml_file_name);

    // config section 
    if (input.contains("config")){
        const auto& config = toml::find(input, "config");
        
        // Get tol 
        auto tol = toml::find_or<real_t>(config, "tolerance", 2.0);
        set_tol(tol);

        // Get max_trails, default is 10000
        m_max_trails = toml::find_or<size_t>(config, "max_trail", 10000);

        // Get rand_seed, default is time(0)
        m_rand_seed = toml::find_or<size_t>(config, "rand_seed", time(0));

        // Get output file for coordinates 
        m_out_file = toml::find_or<std::string>(config, "out_file","out.pdb");

        // Get sanity check flag, (debug only)
        m_sanity = toml::find_or<bool>(config, "check_sanity",false);
    }
    else {
        inception::abort("[config] section missing\n");    
    }

    // Box section 
    std::vector<float> box_xlim;
    std::vector<float> box_ylim;
    std::vector<float> box_zlim;

    if (input.contains("box")){
        const auto& box = toml::find(input, "box");

        // Get box limit 
        box_xlim = toml::find<std::vector<real_t>>(box, "xlim");  
        box_ylim = toml::find<std::vector<real_t>>(box, "ylim");  
        box_zlim = toml::find<std::vector<real_t>>(box, "zlim");  
        
        // Check if input is sane 
        assert (box_xlim.size() == 2 && box_xlim[1] > box_xlim[0]);
        assert (box_ylim.size() == 2 && box_ylim[1] > box_ylim[0]);
        assert (box_zlim.size() == 2 && box_zlim[1] > box_zlim[0]);
    }
    else {
        inception::abort("[box] section missing\n");    
    }
    
    // Set box length 
    m_lx = box_xlim[1] - box_xlim[0];
    m_ly = box_ylim[1] - box_ylim[0];
    m_lz = box_zlim[1] - box_zlim[0];
    
    // Set half of box length for PBC 
    m_hlx = 0.5f*m_lx;
    m_hly = 0.5f*m_ly;
    m_hlz = 0.5f*m_lz;


    // Packing section  
    if (input.contains("packing")){
        const auto& packings = toml::find(input, "packing");

        for (auto i=0; i < packings.size();++i){
            const auto& packing = packings[i];
            
            // Get packing type 
            size_t packing_type=RANDOM; 
            if (packing.contains("location")){
                packing_type = PINNED;
            }
            
            // parse according to type 
            if (packing_type == RANDOM) { 
                    
                // Get file 
                assert(packing.contains("file"));
                auto mol_file =  toml::find<std::string>(packing,"file");

                // Get nmols 
                assert(packing.contains("nitems"));
                auto nmols =  toml::find<size_t>(packing,"nitems");

                // Get xlim  
                std::vector<real_t> mol_xlim=box_xlim;
                if (packing.contains("xlim")){
                    mol_xlim = toml::find<std::vector<real_t>>(packing, "xlim"); 
                }

                // Get ylim  
                std::vector<real_t> mol_ylim=box_ylim;
                if (packing.contains("ylim")){
                    mol_ylim = toml::find<std::vector<real_t>>(packing, "ylim"); 
                }
               
                // Get zlim  
                std::vector<real_t> mol_zlim=box_zlim;
                if (packing.contains("zlim")){
                    mol_zlim = toml::find<std::vector<real_t>>(packing, "zlim"); 
                }

                assert (mol_xlim.size() == 2 && mol_xlim[1] > mol_xlim[0]);
                assert (mol_ylim.size() == 2 && mol_ylim[1] > mol_ylim[0]);
                assert (mol_zlim.size() == 2 && mol_zlim[1] > mol_zlim[0]);

                // set to class 
                real_t bounds[6]; 
                bounds[0] = mol_xlim[0];
                bounds[1] = mol_xlim[1];
                bounds[2] = mol_ylim[0];
                bounds[3] = mol_ylim[1];
                bounds[4] = mol_zlim[0];
                bounds[5] = mol_zlim[1];

                this->add(mol_file,nmols,bounds);
            }

            else if (packing_type==PINNED){ 
                // Get file 
                assert(packing.contains("file"));
                auto mol_file =  toml::find<std::string>(packing,"file");

                // Get pinned location  
                std::vector<real_t> loc;
                if (packing.contains("location")){
                    loc = toml::find<std::vector<real_t>>(packing, "location"); 
                }

                assert(loc.size() == 3);

                // Set to class
                this->pin(mol_file,loc.data());
            }
        }
    }
    else {
        inception::abort("[[packing]] section is  missing\n");    
    }
}
