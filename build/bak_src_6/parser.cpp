#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "units.hpp"
#include "parser.hpp"
#include <cassert>

namespace fs = std::filesystem;

Parser_t::Parser_t(std::string file_name){
    // Get extension 
    const auto ext = fs::path(file_name).extension();
    
    // Check if file is present 
    if (! fs::exists(file_name)){
        throw std::runtime_error("Can not open "+file_name+"\n");
    }
    
    // Call appropriate parser
    if (ext==".pdb") parse_pdb(file_name);
    else if (ext==".gro") parse_gro(file_name);
    else if (ext==".xyz") parse_xyz(file_name);
    else if (ext==".mol") parse_mol(file_name);

}

void Parser_t::parse_pdb(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    // Find number of atoms 
    m_natoms = 0;
    while (file.peek() != EOF){
        getline(file,line); 
        std::string key{line,0,6};
        if (key=="HETATM" || key == "ATOM  "){
            m_natoms++;
        }
    }
    assert(m_natoms > 0);


    // Allocate 
    /* m_pos = new real_t[m_natoms*3]; */
    m_pos.reset(new real_t[m_natoms*3]);
    
    // Read 
    file.clear();
    file.seekg(0); 
    real_t xi,yi,zi;
    size_t i=0;
    while (file.peek() != EOF){
        getline(file,line); 
        std::string key{line,0,6};
        
        if (key=="HETATM" || key == "ATOM  "){
            m_symbols.push_back(line.substr(12,4));
            m_resnames.push_back(line.substr(17,3));
            m_resids.push_back(std::stoi(line.substr(22,4)));

            xi = std::stof(line.substr(30,8));
            yi = std::stof(line.substr(38,8));
            zi = std::stof(line.substr(46,8));

            m_pos[3*i] = xi;
            m_pos[3*i + 1] = yi;
            m_pos[3*i + 2] = zi;
            ++i;
        }
    }
    file.close();
}

void Parser_t::parse_gro(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    getline(file,line); 
    getline(file,line); 
    std::istringstream input(line); 
    input >> m_natoms; 
    m_pos.reset(new real_t[m_natoms*3]);

    real_t xi,yi,zi;
    for (auto i=0u; i<m_natoms; ++i){
        getline(file,line); 
        m_resids.push_back(std::stoi(line.substr(0,5)));
        m_resnames.push_back(line.substr(5,5));
        m_symbols.push_back(line.substr(10,5));
        xi = nm_to_aa * std::stof(line.substr(20,8));
        yi = nm_to_aa * std::stof(line.substr(28,8));
        zi = nm_to_aa * std::stof(line.substr(36,8));

        m_pos[3*i] = xi;
        m_pos[3*i + 1] = yi;
        m_pos[3*i + 2] = zi;
    }

    file.close();
}

void Parser_t::parse_xyz(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    getline(file,line); 
    std::istringstream input(line); 
    input >> m_natoms; 
    getline(file,line); 

    std::string symbol_i; 
    real_t xi,yi,zi;
    m_pos.reset(new real_t[m_natoms*3]);

    for (auto i=0u; i<m_natoms; ++i){
        getline(file,line); 
        std::istringstream input(line); 
        input >> symbol_i >> xi >> yi >> zi;

        m_resids.push_back(1);
        m_resnames.push_back("MOL");
        m_symbols.push_back(symbol_i);

        m_pos[3*i] = xi;
        m_pos[3*i + 1] = yi;
        m_pos[3*i + 2] = zi;
    }

    file.close();
}

void Parser_t::parse_mol(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    getline(file,line); 
    getline(file,line); 
    getline(file,line); 
    getline(file,line); 
    std::istringstream input(line); 
    input >> m_natoms; 
    m_pos.reset(new real_t[m_natoms*3]);

    std::string symbol_i; 
    real_t xi,yi,zi;
    for (auto i=0u; i<m_natoms; ++i){
        getline(file,line); 
        std::istringstream input(line); 
        input  >> xi >> yi >> zi >> symbol_i;

        m_resids.push_back(1);
        m_resnames.push_back("MOL");
        m_symbols.push_back(symbol_i);

        m_pos[3*i] = xi;
        m_pos[3*i + 1] = yi;
        m_pos[3*i + 2] = zi;
    }

    file.close();
}

void Parser_t::print(){
    for (auto i=0u; i<m_natoms; ++i){
        std::cout
        << m_symbols[i]<<" "
        << m_resnames[i]<<" "
        << m_resids[i]<<" "
        << m_pos[3*i]<<" "
        << m_pos[3*i+1]<<" "
        << m_pos[3*i+2]<<"\n";
    }
}


Parser_t::~Parser_t(){
}
