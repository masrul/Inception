#include "box.hpp"
#include "real_type.hpp"
#include "molecule_tracker.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "progressbar.hpp"
#include <memory>
#include <cassert>
#include "writer.hpp"
#include "abort.hpp"
#include "math_util.hpp"
#include "extern/toml11/toml.hpp"

OBox_t::OBox_t(real_t lx, real_t ly, real_t lz,real_t tol): 
    m_lx{lx},
    m_ly{ly},
    m_lz{lz},
    m_tol{tol}
{
    m_hlx = 0.5f*lx;
    m_hly = 0.5f*ly;
    m_hlz = 0.5f*lz;
    m_tol2 = m_tol*m_tol;
    m_nmol_types = 0; 
    m_natoms = 0;
    m_max_trails = 10000;
}


OBox_t::OBox_t(std::string toml_file_name){
    m_nmol_types = 0; 
    m_natoms = 0;
    init_from_toml(toml_file_name); 
}

void OBox_t::init_from_toml(std::string toml_file_name){


    const auto input = toml::parse(toml_file_name);

    // config section 
    if (input.contains("config")){
        const auto& config = toml::find(input, "config");
        m_tol = toml::find_or<real_t>(config, "tolerance", 2.0);
        m_tol2= m_tol*m_tol;
        m_max_trails = toml::find_or<size_t>(config, "max_trail", 10000);
        m_rand_seed = toml::find_or<size_t>(config, "rand_seed", 500);
        m_out_file = toml::find_or<std::string>(config, "out_file","out.pdb");
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
        box_xlim = toml::find<std::vector<real_t>>(box, "xlim");  
        box_ylim = toml::find<std::vector<real_t>>(box, "ylim");  
        box_zlim = toml::find<std::vector<real_t>>(box, "zlim");  

        assert (box_xlim.size() == 2 && box_xlim[1] > box_xlim[0]);
        assert (box_ylim.size() == 2 && box_ylim[1] > box_ylim[0]);
        assert (box_zlim.size() == 2 && box_zlim[1] > box_zlim[0]);
    }
    else {
        inception::abort("[box] section missing\n");    
    }

    m_lx = box_xlim[1] - box_xlim[0];
    m_ly = box_ylim[1] - box_ylim[0];
    m_lz = box_zlim[1] - box_zlim[0];

    m_hlx = 0.5f*m_lx;
    m_hly = 0.5f*m_ly;
    m_hlz = 0.5f*m_lz;


    // Packing section  
    if (input.contains("packing")){
        const auto& packing = toml::find(input, "packing");

        // Pinned packing 
        if(packing.contains("pinned")){
            const auto& packing_pinneds =  toml::find(packing,"pinned");

            for (auto i=0u; i< packing_pinneds.size();++i){
                const auto& packing_pinned = packing_pinneds[i];

                // Get file 
                assert(packing_pinned.contains("file"));
                auto mol_file =  toml::find<std::string>(packing_pinned,"file");

                // Get pinned location  
                std::vector<real_t> loc;
                if (packing_pinned.contains("location")){
                    loc = toml::find<std::vector<real_t>>(packing_pinned, "location"); 
                }

                // Set to class
                assert(loc.size() == 3);
                this->pin(mol_file,loc.data());
            }
        }

        // Random packing 
        if(packing.contains("random")){
            const auto& packing_randoms =  toml::find(packing,"random");

            for (auto i=0u; i< packing_randoms.size();++i){
                const auto& packing_random = packing_randoms[i];
                
                // Get file 
                assert(packing_random.contains("file"));
                auto mol_file =  toml::find<std::string>(packing_random,"file");

                // Get nmols 
                assert(packing_random.contains("nitems"));
                auto nmols =  toml::find<size_t>(packing_random,"nitems");

                // Get xlim  
                std::vector<real_t> mol_xlim=box_xlim;
                if (packing_random.contains("xlim")){
                    mol_xlim = toml::find<std::vector<real_t>>(packing_random, "xlim"); 
                }

                // Get ylim  
                std::vector<real_t> mol_ylim=box_ylim;
                if (packing_random.contains("ylim")){
                    mol_ylim = toml::find<std::vector<real_t>>(packing_random, "ylim"); 
                }
               
                // Get zlim  
                std::vector<real_t> mol_zlim=box_zlim;
                if (packing_random.contains("zlim")){
                    mol_zlim = toml::find<std::vector<real_t>>(packing_random, "zlim"); 
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
        }
    }
    else {
        inception::abort("[packing] section missing\n");    
    }

    // Check atleast one packing element is requested 
    if (m_mol_types.size() == 0){
        inception::abort("Please provide atleast one moleule to pack\n");    
    }
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

    ++m_nmol_types;
}

void OBox_t::pin(std::string file_name, real_t* loc){

    // Add mol_type 
    m_mol_types.emplace_back(file_name,static_cast<size_t>(1));
    
    // set bounds 
    auto& mol_type = m_mol_types[m_nmol_types];
    mol_type.m_xmin = loc[0];
    mol_type.m_xmax = loc[1];
    mol_type.m_ymin = loc[1];
    mol_type.m_ymax = loc[1];
    mol_type.m_zmin = loc[2];
    mol_type.m_zmax = loc[2];

    ++m_nmol_types;
}

void OBox_t::pack(){

    // Find total atoms
    size_t tmols=0;
    for (auto const& mol_type : m_mol_types){
        m_natoms += (mol_type.m_natoms * mol_type.m_nmols);
        tmols += mol_type.m_nmols;
    }

    // Allocate memroy for position 
    /* m_pos.reset(new real_t[m_natoms*3]); */
    m_pos=new real_t[m_natoms*3];

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

    this->rank_packing();
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
                

                // Move to posx,posy,posz 
                mol_type.move_to(posx,posy,posz);


                // check overlap 
                auto overlap = check_overlap(mol_type); 

                if (!overlap){
                    //copy to global pos 
                    mol_type.copy_to(&m_pos[3*ninserts_atom]);
                    m_mol_trackers.emplace_back(mol_type_id,mol_type,&m_pos[3*ninserts_atom]);
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
    this->write(m_out_file);
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
    Writer_t writer{*this,file_name};
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

    if (sqrt(rmin) < m_tol){
        std::cout<<"\nSanity check failed with rmin:"<< sqrt(rmin)<<"\n";
    }
    else { 
        std::cout<<"\nSanity check passed!\n";
    }
}

void OBox_t::rank_packing(){
    std::vector<size_t> scores; 
    size_t max_natoms = std::numeric_limits<size_t>::min(); 
 
    for (const auto& mol_type : m_mol_types){
        const auto& natoms = mol_type.m_natoms; 
        scores.push_back(natoms); 
        if (natoms > max_natoms) max_natoms = natoms;
    }
    m_packing_order = MathUtil::argsort(scores);
}
