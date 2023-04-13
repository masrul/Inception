#ifndef MATH_UTIL_HPP

#define MATH_UTIL_HPP

#include "real_type.hpp"
#include "stdio.h"
#include <vector>

namespace MathUtil {

void matmul(const real_t*, const real_t*, real_t*, size_t, size_t, size_t);

void get_rot_mat(real_t alpha, real_t beta, real_t gamma, real_t* rot_mat);

// Hello
std::vector<size_t> argsort(const std::vector<size_t>&);


} // End of MathUtil


#endif /* end of include guard MATH_UTIL_HPP */

