# Inception : A moleuclar packing tool, faster and periodic

This is a molecular packing software similar to Packmol. But it is aimed to be faster, and obey PBC. 

## Table of contents 
- [Installation](#installation)

## Installation 
#### Requirement: 
+ A modern C++ compiler (C++-17) and 
+ cmake

Built is tested with GCC-11 and Clang-14. 

```bash 
git clone https://github.com/masrul/Inception.git
cd Inception 
mkdir build && cd build 
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/installation
make && make install 
```

## How to use 
**toml** file is used to interact with the tool. 

A simple example toml file to pack 5000 water molecules into 100 $\AA$
 
```toml 
[config]
    tolerance = 3.0
    out_file = "out.gro"

[box]
    xlim = [0.0,100.0]
    ylim = [0.0,100.0]
    zlim = [0.0,100.0]

[[packing]]
    file="water.pdb"
    nitems = 5000
```


<p align="center">
  <img width="400" height="400" src="images/pack_water.jpg">
  <br>
  Fig-1: Packing 5000 water molecules using Inception.
</p>

<p align="center">
  <img width="400" height="400" src="images/interface.jpg">
</p>

<p align="center">
  <img width="400" height="400" src="images/pinned.jpg">
</p>
(Under construction)
