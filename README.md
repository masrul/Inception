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

<p align="center">
  <img width="300" height="300" src="images/pack_water.jpg">
  <br>
  Fig-1: Packing 5000 water molecules using Inception.
</p>

**toml** file mainly has three sections, 

+ **[config]**
	+ tolerance = minimium distance between atoms 
	+ out_file = coordinate file to write output, currently gro, pdb and xyz is supported 
	+ max_trails = Maximum number of trails to insert a molecules, default is 1000 
	+ rand_seed = Seed for random number, default is based on OS time. 

+ **[box]** 
	+ **xlim** = Limit of box edge along $X$ direction 
	+ **ylim** = Limit of box edge along $Y$ direction 
	+ **zlim** = Limit of box edge along $Z$ direction 

There are two types of packing supported currently, 
### Type-1: Random insertion 
+ **[[packing]]**  
	+ **file** = Coordinate file of molecule [gro, pdb, xyz, and mol]. 
	+ **nitems** = Number of molecules. 
	+ xlim = Limit molecule center along $X$ direction. 
	+ ylim = Limit molecule center along $X$ direction.
	+ zlim = Limit molecule center along $X$ direction. 

Using the packing limit, we can restrict molecular packing to certain region. For example, we can create interface of water and decane. 

```toml 
[config]
    tolerance = 3.0
    out_file = "out.gro"

[box]
    xlim = [0.0,150.0]
    ylim = [0.0,150.0]
    zlim = [0.0,200.0]

[[packing]]
    file="water.pdb"
    nitems = 10000
    zlim = [0.0,100.0]

[[packing]]
    file="decane.gro"
    nitems = 2000
    zlim = [100.0,200.0]
```

<p align="center">
  <img width="300" height="300" src="images/interface.jpg">
  <br>
  Fig-2: Creating interface between water and decane. 
</p>

### Type-2: Pinned insertion 
 
**[[packing]]**

+ **file** = Coordinate file of molecule [gro, pdb, xyz, and mol].
+ **location** = Where to pin the center-of-mass. 

User can pinned the center-of-mass of molecule into the certain location. 
Here is an example of pinning a macromolecule into the center of a box.

```toml 
[config]
    tolerance = 3.0
    out_file = "out.gro"

[box]
    xlim = [0.0,100.0]
    ylim = [0.0,100.0]
    zlim = [0.0,100.0]

[[packing]]
    file = "lignin.gro"
    location = [50.0,50.0,50.0]

[[packing]]
    file="water.pdb"
    nitems = 8000
```

<p align="center">
  <img width="300" height="300" src="images/pinned.jpg">
  <br>
  Fig-3: Pinning Lignin into a box full of water.  

</p>
(Under construction)
