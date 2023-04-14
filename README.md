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

A simple example toml file to pack 5000 water molecules into 100 angstrom cubic box. Fig-1 shows the packing. 
 
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

**toml** file mainly has three sections, 

+ [config]
	+ tolerance = minimium distance between atoms 
	+ out_file = coordinate file to write output, currently gro,pdb and xyz is supported 
	+ max_trails = Maximum number of trails to insert a molecules, default is 1000 
	+ rand_seed = Seed for random number, default is based on OS time. 

+ [box] 
	+ xlim = Limit of box edge along $X$ direction 
	+ ylim = Limit of box edge along $Y$ direction 
	+ zlim = Limit of box edge along $Z$ direction 
+ [[packing]]




<p align="center">
  <img width="300" height="300" src="images/pack_water.jpg">
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
