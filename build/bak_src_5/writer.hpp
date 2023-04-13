#ifndef WRITER_HPP

#define WRITER_HPP

#include "box.hpp"
#include <memory>

class Writer_t {
public: 
    Writer_t(const OBox_t& box,std::string);
    
    const OBox_t& m_box; 
    std::string m_file_name;

private:
    // Attributes 
    std::string m_ext; 
    /* real_t* m_pos; */
    std::shared_ptr<real_t[]>m_pos;
    size_t m_natoms;
    std::vector<std::string> m_resnames;
    std::vector<std::string> m_symbols;
    std::vector<size_t> m_resids; 
    std::vector<size_t> m_atomids; 
    real_t m_lx,m_ly,m_lz;
    
    void preprocess(); // Fix resids, atomsids etc
    void write_pdb();
    void write_gro();
    void write_xyz();

};


#endif /* end of include guard WRITER_HPP */

