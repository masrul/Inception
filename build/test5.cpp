/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Thursday Apr 06, 2023 01:43:17 CDT
 */

#include <iostream>

using std::cout; 
using std::cin; 
using std::endl; 

void test(float* arr){

}

int main()
{
    
    std::shared_ptr<float[]> data(new float[9]);
    test(data.get());
    return 0;
}

