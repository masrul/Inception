#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "units.hpp"
#include "parse.hpp"

namespace fs = std::filesystem;

Parser::Parser(std::string file_name){
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

void Parser::parse_pdb(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    m_natoms = 0;
    while (file.peek() != EOF){
        getline(file,line); 
        std::string key{line,0,6};
        
        if (key=="HETATM" || key == "ATOM  "){
            m_natoms++;
            m_symbols.push_back(line.substr(12,4));
            m_resnames.push_back(line.substr(17,3));
            m_resids.push_back(std::stoi(line.substr(22,4)));
            m_x.push_back(std::stof(line.substr(30,8)));
            m_y.push_back(std::stof(line.substr(38,8)));
            m_z.push_back(std::stof(line.substr(46,8)));
        }
    }

    file.close();
}

void Parser::parse_gro(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    getline(file,line); 
    getline(file,line); 
    std::istringstream input(line); 
    input >> m_natoms; 

    for (auto i=0u; i<m_natoms; ++i){
        getline(file,line); 
        m_resids.push_back(std::stoi(line.substr(0,5)));
        m_resnames.push_back(line.substr(5,5));
        m_symbols.push_back(line.substr(10,5));
        m_x.push_back(nm_to_aa * std::stof(line.substr(20,8)));
        m_y.push_back(nm_to_aa * std::stof(line.substr(28,8)));
        m_z.push_back(nm_to_aa * std::stof(line.substr(36,8)));
    }

    file.close();
}

void Parser::parse_xyz(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    getline(file,line); 
    std::istringstream input(line); 
    input >> m_natoms; 
    getline(file,line); 

    std::string symbol_i; 
    real_t x_i,y_i,z_i;
    for (auto i=0u; i<m_natoms; ++i){
        getline(file,line); 
        std::istringstream input(line); 
        input >> symbol_i >> x_i >> y_i >> z_i;

        m_resids.push_back(1);
        m_resnames.push_back("MOL");
        m_symbols.push_back(symbol_i);
        m_x.push_back(x_i);
        m_y.push_back(y_i);
        m_z.push_back(z_i);
    }

    file.close();
}

void Parser::parse_mol(const std::string &file_name){
    std::ifstream file{file_name};
    std::string line; 
    
    getline(file,line); 
    getline(file,line); 
    getline(file,line); 
    getline(file,line); 
    std::istringstream input(line); 
    input >> m_natoms; 

    std::string symbol_i; 
    real_t x_i,y_i,z_i;
    for (auto i=0u; i<m_natoms; ++i){
        getline(file,line); 
        std::istringstream input(line); 
        input  >> x_i >> y_i >> z_i >> symbol_i;

        m_resids.push_back(1);
        m_resnames.push_back("MOL");
        m_symbols.push_back(symbol_i);
        m_x.push_back(x_i);
        m_y.push_back(y_i);
        m_z.push_back(z_i);
    }

    file.close();
}

void Parser::print(){
    for (auto i=0u; i<m_natoms; ++i){
        std::cout
        << m_symbols[i]<<" "
        << m_resnames[i]<<" "
        << m_resids[i]<<" "
        << m_x[i]<<" "
        << m_y[i]<<" "
        << m_z[i]<<"\n";
    }
}

