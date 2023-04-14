#include <iostream>
#define pi 3.1416 
#define real_t float

void matmul(const real_t* A, const real_t* B, real_t* C, size_t n1, size_t n2, size_t n3){
    for (size_t i = 0; i < n1; ++i) {
        for (size_t j = 0; j < n3; ++j) {
            C[i*n3 + j] = 0.0f;
            for (size_t k = 0; k < n2; ++k) {
                C[i*n3 + j] += A[i*n2 + k] * B[k*n3 + j];
            }
        }
    }
}


int main()
{
    real_t A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    real_t B[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    real_t* C = new real_t[9];
    matmul(A,B,C,3,3,3);
    
    for (auto i=0u; i<9; ++i)
        std::cout<<C[i]<<"\n";

    /* real_t A[] = {0, 1, 2, 3, 4, 5}; */
    /* real_t B[] = {0, 1, 2, 3, 4, 5}; */
    /* real_t* C = new real_t[9]; */
    /* matmul(A,B,C,3,2,3); */
    
    /* for (auto i=0u; i<9; ++i) */
    /*     std::cout<<C[i]<<"\n"; */


    return 0;
}

