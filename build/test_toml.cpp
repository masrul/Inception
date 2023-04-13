/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Saturday Apr 08, 2023 23:04:59 CDT
 */

#include <iostream>
#include <cassert>
#include "toml11/toml.hpp"

using std::cout; 
using std::cin; 
using std::endl; 

int main(){
    const auto input = toml::parse("input.toml");
    std::cout<<input<<"\n";

    // config section 
    float tolerance;
    size_t max_trails;
    size_t rand_seed;
    std::string out_file;
    if (input.contains("config")){
        const auto& config = toml::find(input, "config");
        tolerance = toml::find_or<float>(config, "tolerance", 3.0);
        max_trails = toml::find_or<size_t>(config, "max_trail", 10000);
        rand_seed = toml::find_or<size_t>(config, "rand_seed", 500);
        out_file = toml::find_or<std::string>(config, "out_file","out.pdb");
    }
    else {
        std::cout<<"[config] section missing\n";
        std::cout<<"Quitting ..";
        std::terminate();
    }

    // Box section 
    std::vector<float> xlim;
    std::vector<float> ylim;
    std::vector<float> zlim;

    if (input.contains("box")){
        const auto& box = toml::find(input, "box");
        xlim = toml::find<std::vector<float>>(box, "xlim");  
        ylim = toml::find<std::vector<float>>(box, "ylim");  
        zlim = toml::find<std::vector<float>>(box, "zlim");  

        assert (xlim.size() == 2 && ylim.size() == 2 && zlim.size() == 2);
    }
    else {
        std::cout<<"[config] section missing\n";
        std::cout<<"Quitting ..\n";
        std::terminate();
    }

    std::cout<<xlim[0]<<" "<<xlim[1]<<"\n";
    std::cout<<ylim[0]<<" "<<ylim[1]<<"\n";
    std::cout<<zlim[0]<<" "<<zlim[1]<<"\n";


    // Packing section  
    if (input.contains("packing")){
        const auto& packing = toml::find(input, "packing");
        if(packing.contains("random")){
            std::cout<<"Hello\n";
            const auto& randoms =  toml::find(packing,"random");
            const auto& random = randoms[0];

        }
    }
    else {
        std::cout<<"[packing] section missing\n";
        std::cout<<"Quitting ..\n";
        std::terminate();
    }


    return 0;
}

/* int main() */
/* { */
    
/*     toml::table input = toml::parse_file("input.toml"); */
/*     /1* std::cout<<input<<"\n"; *1/ */

    
/*     // config section */ 
/*     float tolerance; */
/*     size_t max_trails; */
/*     size_t rand_seed; */
/*     std::string out_file; */
/*     if (input.contains("config")){ */
/*         const auto& config = input["config"]; */
/*         tolerance = input["config"]["tolerance"].value_or(3.0f); */
/*         max_trails = input["config"]["max_trails"].value_or(10000); */
/*         rand_seed = input["config"]["rand_seed"].value_or(500); */
/*         out_file = input["config"]["out_file"].value_or("out.pbd"); */
/*     } */
/*     else { */
/*         std::cout<<"[config] section missing\n"; */
/*         std::cout<<"Quitting .."; */
/*         std::terminate(); */
/*     } */

/*     /1* std::cout<<tolerance<<" "<<max_trails<<" "<<rand_seed<<" "<<out_file<<"\n"; *1/ */

/*     // Box */ 
/*     float xlim[2]; */ 
/*     if (input.contains("box")){ */
/*         auto xlim_arr = input["box"]["xlim"]; */
/*         /1* xlim[0]=xlim_arr[0]; *1/ */
/*         /1* /2* std::cout<<xlim_arr->size()<<"\n"; *2/ *1/ */
/*         /1* /2* std::cout<<xlim_arr[0]<<"\n"; *2/ *1/ */
/*         /1* /2* std::cout<<xlim_arr[1]<<"\n"; *2/ *1/ */
/*         /1* /2* std::cout<<xlim_arr[2]<<"\n"; *2/ *1/ */
/*         /1* /2* std::cout<<xlim_arr[3]<<"\n"; *2/ *1/ */
/*         /1* /2* std::cout<<xlim_arr[4]<<"\n"; *2/ *1/ */
/*         /1* /2* std::cout<<xlim_arr[5]<<"\n"; *2/ *1/ */

/*         /1* /2* xlim[0] = xlim_arr[0]; *2/ *1/ */
/*         for (auto&& [k, v] : xlim_arr) */
/*         { */
/*             // ... */
/*         } */
/*     } */
/*     else { */
/*         std::cout<<"[box] section missing\n"; */
/*         std::cout<<"Quitting .."; */
/*         std::terminate(); */
/*     } */


/*     return 0; */
/* } */

