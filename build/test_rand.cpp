/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Tuesday Apr 11, 2023 04:18:52 CDT
 */

#include <iostream>
#include "rand_engine.hpp"
#include "timer.hpp"

using std::cout; 
using std::cin; 
using std::endl; 

int main()
{

    RandEngine rnd;
    
    Timer timer{}; 

    for (auto i=0; i<100000000; ++i){
        auto r = rnd();
    }
    
    timer.print_elapsed_time();

    timer.reset(); 

    for (auto i=0; i<100000000; ++i){
        auto r = ((float) std::rand() / (RAND_MAX));
    }
    timer.print_elapsed_time();





    
    return 0;
}

