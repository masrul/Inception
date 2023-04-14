/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Friday Apr 07, 2023 06:07:09 CDT
 */

#include <iostream>
#include <string>
#include <memory>

using std::cout; 
using std::cin; 
using std::endl; 

int main()
{
   
    auto a = std::shared_ptr<float[]>(new float[3]); 
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;

    float* b = new float[3]; 
    std::memcpy(b,&a[0],3*sizeof(float)); 
    b[0]=-1;

    std::cout<<a[0] << "\n";


    
    return 0;
}

