#include "math_util.hpp"
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>


namespace MathUtil {

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


void get_rot_mat(real_t alpha, real_t beta, real_t gamma, real_t* rot_mat){

    auto cos_alpha = cos(alpha);
    auto sin_alpha = sin(alpha);

    auto cos_beta = cos(beta);
    auto sin_beta = sin(beta);

    auto cos_gamma = cos(gamma);
    auto sin_gamma = sin(gamma);

    
    // First row
    rot_mat[0] = cos_beta * cos_gamma;
    rot_mat[1] = sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma;
    rot_mat[2] = cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma;

    // Second row
    rot_mat[3] = cos_beta * sin_gamma;
    rot_mat[4] = sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma;
    rot_mat[5] = cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma;

    // Third row
    rot_mat[6] = -sin_beta;
    rot_mat[7] = sin_alpha * cos_beta;
    rot_mat[8] = cos_alpha * cos_beta;
}


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

} // End of MathUtil 

