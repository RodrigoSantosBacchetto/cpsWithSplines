/* 
 * File:   CpsMatrix.cpp
 * Author: Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 */

#include "CpsMatrix.hpp"


namespace cvx {

    CpsMatrix::CpsMatrix(const CpsMatrix& src) :
    _cps(src._cps) {
        // NOOP
    }

    CpsMatrix::CpsMatrix(CpsMatrix&& tmp) :
    _cps(std::move(tmp._cps)) {
        // NOOP
    }

    CpsMatrix::CpsMatrix(const matrix_t& data) :
    _cps(data) {
        // NOOP
    }

    CpsMatrix::CpsMatrix(matrix_t&& data) :
    _cps(std::move(data)) {
        // NOOP
    }

    CpsMatrix CpsMatrix::assign(const CpsMatrix& src) {
        return (_cps = src._cps, *this);
    }



} // namespace cvx
