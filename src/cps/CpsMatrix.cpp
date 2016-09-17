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

    CpsMatrix& CpsMatrix::assign(const CpsMatrix& src) {
        return (_cps = src._cps, *this);
    }

    CpsMatrix& CpsMatrix::assign(CpsMatrix&& tmp) {
        return (_cps = std::move(tmp._cps), *this);
    }

    /**
     * @brief   Implements OpenCV XML/YAML serialization.
     * @param[in,out] fs    Reference to a `FileStorage` object.
     * @param[in] name      Reference to root element name.
     */
    void CpsMatrix::write(FileStorage& fs, const String&) const {
        // Serialize contour data.
        fs << _cps;
    }

    /**
     * @brief   Implements OpenCV XML/YAML deserialization.
     * @param[in,out] fs    Reference to a `FileStorage` object.
     */
    void CpsMatrix::read(const FileNode& fn, const CpsMatrix&) {
        // Deserialize the contour data.
        fn >> _cps;
    }


} // namespace cvx
