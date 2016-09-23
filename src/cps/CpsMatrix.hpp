/**
 * @file        CpsMatrix.hpp
 *
 * @brief       Defines a class that holds a matrix in which each row represents
 *              the Contour-Point signature (point descriptor) of points sampled
 *              from a contour.
 *
 * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 *
 * @copyright   Copyright © 2015, National University of Asunción, all rights
 *              reserved.
 *
 * This file is part of The Computer Vision Tools Project. It is subject to the
 * license terms in the [LICENSE.md](../license/LICENSE.md) file found in the
 * `licenses` sub-folder of the top-level directory of this distribution and at
 * https://bitbucket.org/cpslccauna/CPSignature/license/LICENSE.md. No part of
 * this project, including this file, may be copied, modified, propagated, or
 * distributed except according to the terms contained in the above LICENSE
 * file.
 *
 * See the [COPYING.md](../COPYING.md) file at the top-level directory of this
 * distribution and at https://bitbucket.org/cpslccauna/CPSignature/COPYING.md
 * for more legal information.
 */

#ifndef CbVX_DESCRIPTOR_CPSMATRIX_HPP__INCLUDED
#define CbVX_DESCRIPTOR_CPSMATRIX_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Persistence.hpp"

namespace cvx {

    class CpsMatrix {
    public:
        
        typedef Matrix1r matrix_t;

        CpsMatrix() {
            // NOOP
        }

        CpsMatrix(const CpsMatrix& src);

        CpsMatrix(CpsMatrix&& tmp);

        CpsMatrix(const matrix_t& cps);

        CpsMatrix(matrix_t&& cps);

        CpsMatrix operator=(const CpsMatrix& src) {
            return assign(src);
        }

        CpsMatrix operator=(CpsMatrix&& tmp) {
            return assign(std::move(tmp));
        }

        CpsMatrix assign(const CpsMatrix& src);

        CpsMatrix& assign(CpsMatrix&& tmp);

        ~CpsMatrix() {
            // NOOP
        }

        /**
         * @brief   Returns the number of points sampled from the source contour,
         *          the sampling size, or the number of signatures contained in the
         *          signature matrix.
         * @return  An unsigned integer with the number of signatures, or sampling
         *          size on the source contour.
         */
        unsigned sampling() const {
            return _cps.rows;
        }

        /**
         * @brief   Returns the sampling size, the definition, of the signatures
         *          contained in the signature matrix.
         * @return  An unsigned integer with the number of signatures, or sampling
         *          size on the source contour.
         */
        unsigned definition() const {
            return _cps.cols;
        }

        /**
         * @brief   Accessor for the wrapped matrix data.
         * @return  Returns the signature matrix.
         */
        const matrix_t& data() const {
            return _cps;
        }

    private:

        Matrix1r _cps;

    };

    // Comparison operators

    inline
    bool operator==(const CpsMatrix& lc, const CpsMatrix& rc) {
        return countNonZero(lc.data() != rc.data()) == 0;
    }

    inline
    bool operator!=(const CpsMatrix& lc, const CpsMatrix& rc) {
        return !(lc == rc);
    }

} // namespace cvx


#endif // CbVX_DESCRIPTOR_CPSMATRIX_HPP__INCLUDED
