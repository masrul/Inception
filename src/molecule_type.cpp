#include <iostream>
#include "real_type.hpp"
#include "parser.hpp"
#include "molecule_type.hpp"
#include "units.hpp"
#include "math_util.hpp"
#include <fstream>


MoleculeType_t::MoleculeType_t(std::string file_name) {
    Parser_t prs{file_name};
    prs.print();
    
    // Copy trivial stuffs 
    m_natoms = prs.m_natoms;
    m_symbols = prs.m_symbols; 
    m_resnames = prs.m_resnames;


    std::vector<std::string> m_symbols;
    std::vector<std::string> m_resnames;
    std::vector<size_t> m_resids; 

    find_center_atom(prs.m_pos);
    create_rand_structs(prs.m_pos);
    write();

}

void MoleculeType_t::create_rand_structs(real_t* pos){

    // Randomly rotate and save 
    m_all_pos = new real_t[m_nrand_structs*m_natoms*3];
    real_t* rot_mat = new real_t[9];
    for (auto i=0u; i<m_nrand_structs;++i){
        auto r0 = ((real_t) rand() / (RAND_MAX)) + 1;
        auto r1 = ((real_t) rand() / (RAND_MAX)) + 1;
        auto r2 = ((real_t) rand() / (RAND_MAX)) + 1;
        auto alpha = (2*r0-1)*pi;
        auto beta = (2*r1-1)*pi;
        auto gamma = (2*r2-1)*pi;

        m_pos = &m_all_pos[m_natoms*3*i];
        MathUtil::get_rot_mat(alpha,beta,gamma,rot_mat);
        MathUtil::matmul(pos,rot_mat,m_pos,m_natoms,3,3);
    }
}

void MoleculeType_t::find_center_atom(real_t* pos){
    // Find center  
    real_t xcom = 0.0f;
    real_t ycom = 0.0f;
    real_t zcom = 0.0f;
    
    for (auto i=0u;i<m_natoms;++i){
        xcom += pos[3*i];
        ycom += pos[3*i+1];
        zcom += pos[3*i+2];
    }
    xcom /=m_natoms;
    ycom /=m_natoms;
    zcom /=m_natoms;

    // Find which atom is closer to center  
    size_t m_cIDx;
    real_t mindist = std::numeric_limits<real_t>::max(); 
    for (auto i=0u;i<m_natoms;++i){
        auto dx = pos[3*i] - xcom; 
        auto dy = pos[3*i+1] - ycom; 
        auto dz = pos[3*i+2] - zcom; 
        auto dist = dx*dx + dy*dy + dz*dz; 

        if (dist < mindist){
            mindist = dist;
            m_cIDx = i; 
        }
    }

    // Move center atom to origin 
    xcom = pos[3*m_cIDx]; 
    ycom = pos[3*m_cIDx+1]; 
    zcom = pos[3*m_cIDx+2]; 

    for (auto i=0u;i<m_natoms;++i){
        pos[3*i] -=xcom;
        pos[3*i+1] -=ycom;
        pos[3*i+2] -=zcom;
    }

    // Find max distance from center atom to any atom 
    real_t m_rmax = std::numeric_limits<real_t>::min(); 
    for (auto i=0u;i<m_natoms;++i){
        auto dx = pos[3*i]; 
        auto dy = pos[3*i+1]; 
        auto dz = pos[3*i+2]; 
        auto dist = dx*dx + dy*dy + dz*dz; 

        if (dist > m_rmax){
            m_rmax = dist;
        }
    }
}

void MoleculeType_t::get_rand_struct(){
    auto r = ((real_t) rand() / (RAND_MAX)) + 1;
    size_t rand_idx = round((m_nrand_structs-1)*r); 

    m_pos = &m_all_pos[m_natoms*3*rand_idx];
}

void MoleculeType_t::write(){
    // Write for test
    std::ofstream file{"all.xyz"};

    for (auto i=0u; i<m_nrand_structs;++i){
        m_pos = &m_all_pos[m_natoms*3*i];
        
        file<<m_natoms<<"\n"<<"\n";
        for (auto j=0u;j<m_natoms;++j){
            file
                <<m_symbols[j]<<" "
                <<m_pos[3*j] << " "
                <<m_pos[3*j+1] << " "
                <<m_pos[3*j+2] << "\n";
        }
    }
    file.close();
}

