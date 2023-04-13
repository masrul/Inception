/**
 * Author      : masrulhuda (masrulhuda@Masruls-MBP)
 * Host        : Masruls-MBP
 * Created     : Monday Apr 10, 2023 05:23:39 CDT
 */

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>


using std::cout; 
using std::cin; 
using std::endl; 

std::vector<size_t> argsort(const std::vector<size_t> &array) {
    std::vector<size_t> indices(array.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
        [&array](int left, int right) -> bool {
          // sort indices according to corresponding array element
          return array[left] > array[right];
        });

    return indices;
}

int main()
{
    std::vector<size_t> a = {1,2,3,4,4}; 

    auto indices = argsort(a);

    for (const auto& i : indices){
        std::cout<<i<<" ";
    }
    std::cout<<"\n";
    return 0;
}

