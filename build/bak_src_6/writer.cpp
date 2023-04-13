#include "writer.hpp"
#include "box.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "units.hpp"
#include <cassert>
#include <iomanip>

// For formatting 
using std::setw; 
using std::fixed; 
using std::setprecision; 
using std::right; 
using std::left; 
using std::endl; 


namespace fs = std::filesystem;

void wrap_around(size_t& num, const size_t& max_num){
    num =  (num - 1) % max_num + 1;
}

Writer_t::Writer_t(const OBox_t& box, std::string file_name):
    m_box{box},
    m_file_name{file_name}
    
{
    // Get extension 
    m_ext = fs::path(m_file_name).extension();
    
    // preprocess 
    preprocess();

    // Call appropriate parser
    if (m_ext==".pdb") write_pdb();
    else if (m_ext==".gro") write_gro();
    else if (m_ext==".xyz") write_xyz();
}

void Writer_t::preprocess(){

    // set max atomid and resid 
    size_t max_resid = 99999u;
    size_t max_atomid = 99999u;
    real_t factor = 1.0;
    if (m_ext==".pdb"){
        max_resid = 9999u;
        max_atomid = 99999u;
    }
    else if (m_ext==".gro"){
        max_resid = 99999u;
        max_atomid = 99999u;
        factor = 0.10;
    }

    // Collect from box 
    m_pos.reset(new real_t[3*m_box.m_natoms]);
    size_t atomid = 0u;
    size_t prev_max_resid = 0u;
    m_natoms = m_box.m_natoms;

    for (auto type_id=0; type_id<m_box.m_nmol_types;++type_id){
        for (const auto& mol_tracker : m_box.m_mol_trackers){
            if (mol_tracker.m_mol_type_id != type_id) continue;

            auto& mol_type = mol_tracker.m_mol_type; 
            auto& symbols = mol_type.m_symbols; 
            auto& resnames = mol_type.m_resnames; 
            auto& natoms = mol_type.m_natoms;
            auto& resids = mol_type.m_resids;
            auto& pos = mol_tracker.m_pos;

            for (auto i=0; i<natoms;++i){
                auto atomid_ = ++atomid; 
                auto resid_ = resids[i]+prev_max_resid; 
                wrap_around(atomid_,max_atomid);
                wrap_around(resid_,max_resid);

                m_atomids.push_back(atomid_);
                m_resids.push_back(resid_);
                m_symbols.push_back(symbols[i]);
                m_resnames.push_back(resnames[i]);

                auto ii = atomid - 1; 
                m_pos[3*ii] = factor*pos[3*i];
                m_pos[3*ii+1] = factor*pos[3*i+1];
                m_pos[3*ii+2] = factor*pos[3*i+2];
            }
            prev_max_resid = *max_element(m_resids.begin(), m_resids.end());
        }
    }
    
    assert(m_natoms = m_symbols.size());

    // box dim 
    m_lx = factor * m_box.m_lx;
    m_ly = factor * m_box.m_ly;
    m_lz = factor * m_box.m_lz;
    std::cout<<"Completed preprocessing"<<std::endl;
}


void Writer_t::write_pdb() {

    std::ofstream file{m_file_name};

    file << "REMARK Created by Inception"<<endl;

    for (size_t i=0;i < m_natoms; ++i){
        file << setw(6) << left << "ATOM";
        file << setw(5) << right << m_atomids[i]<<" ";
        file << setw(4) << left << m_symbols[i];
        file << setw(3) << right << m_resnames[i]<<"  "; 
        file << setw(4) << right << m_resids[i]<<"    "; 
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i+1];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i+2];
        file << endl; 
    }

    file.close();

}

void Writer_t::write_gro() {

    std::ofstream file{m_file_name};

    file << "Created by Inception"<<endl;
    file << setw(10) <<left<<m_natoms<<endl;

    for (size_t i=0;i < m_natoms; ++i){
        file << setw(5) << right << m_resids[i]; 
        file << setw(5) << right << m_resnames[i]; 
        file << setw(5) << right << m_symbols[i];
        file << setw(5) << right << m_atomids[i];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i+1];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i+2];
        file << endl; 
    }

    file << fixed<< setw(10) << setprecision(5)<<right << m_lx;
    file << fixed<< setw(10) << setprecision(5)<<right << m_ly;
    file << fixed<< setw(10) << setprecision(5)<<right << m_lz;
    file << "\n";

    file.close();

}

void Writer_t::write_xyz() {
    std::ofstream file{m_file_name};

    file << setw(10) <<left<<m_natoms<<endl;
    file << "Created by Inception"<<endl;

    for (size_t i=0;i < m_natoms; ++i){
        file << setw(5) << right << m_symbols[i];
        file << setw(5) << right << "  ";
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i+1];
        file << fixed<< setw(8) << setprecision(3)<<right << m_pos[3*i+2];
        file << endl; 
    }
    file.close();

}




