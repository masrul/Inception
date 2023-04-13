#ifndef WRITER_HPP

#define WRITER_HPP

#include "box.hpp"
#include <memory>
#include <algorithm>

class Writer_t {
public: 
    
    // Attributes 
    const OBox_t& m_box; 
    std::string m_file_name; 

    // Methods 
    Writer_t(const OBox_t& box,std::string);

private:
    // Attributes 
    std::string m_ext; 
    std::shared_ptr<real_t[]>m_pos;
    size_t m_natoms;
    std::vector<std::string> m_resnames;
    std::vector<std::string> m_symbols;
    std::vector<size_t> m_resids; 
    std::vector<size_t> m_atomids; 
    real_t m_lx,m_ly,m_lz;
    
    // Methods 
    void preprocess(); // Fix resids, atomsids etc
    void write_pdb();
    void write_gro();
    void write_xyz();
};


#endif /* end of include guard WRITER_HPP */

