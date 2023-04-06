/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Thursday Apr 06, 2023 00:28:33 CDT
 */

#include <iostream>
#include <memory>

using std::cout; 
using std::cin; 
using std::endl; 

void test(std::shared_ptr<float[]>& data){
    std::cout<<"Entering func\n";
    std::shared_ptr<float[]> d0(data, &data[3]);
    std::cout<<data.use_count()<<"\n";
    std::cout<<"Exiting func\n";
}

int main()
{

    /* std::shared_ptr<float[]> data(new float[9]); */
    // more efficient but requires C++20
    std::shared_ptr<float[]> data = std::make_shared<float[]>(9);
    std::shared_ptr<float[]> d0(data, &data[3]);
    std::shared_ptr<float[]> d1(data, &data[9]);
    std::shared_ptr<float[]> d2(data, &data[1]);

    std::cout<<data.use_count()<<"\n";

    test(data);



    return 0;
}

