# Inception : A moleuclar packing tool, faster and periodic

This is a molecular packing software similar to Packmol. But it is aimed to be faster, and obey PBC. 

## Table of contents 
- [Installation](#installation)

### Installation 
Requirement: A modern C++ compiler (C++-17) and cmake. Built is tested with GCC-11 and Clang-14. 

```bash 
git clone https://github.com/masrul/Inception.git
cd Inception 
mkdir build && cd build 
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/installation
make && make install 
```


<p align="center">
  <img width="400" height="400" src="images/pack_water.jpg">
</p>

<p align="center">
  <img width="400" height="400" src="images/interface.jpg">
</p>

<p align="center">
  <img width="400" height="400" src="images/pinned.jpg">
</p>
(Under construction)
