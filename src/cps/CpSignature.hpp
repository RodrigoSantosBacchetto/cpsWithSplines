/**
 * @file        CpSignature.hpp
 *
 * @brief       Defines a Contour-Point signature (point descriptor)
 *              computing functor.
 *
 * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 *
 * @copyright   Copyright © 2015, National University of Asunción, all rights
 *              reserved.
 *
 * This file is part of The Computer Vision Tools Project. It is subject to the
 * license terms in the [LICENSE.md](../license/LICENSE.md) file found in the
 * `licenses` sub-folder of the top-level directory of this distribution and at
 * https://bitbucket.org/cpslccauna/CpSignature/license/LICENSE.md. No part of
 * this project, including this file, may be copied, modified, propagated, or
 * distributed except according to the terms contained in the above LICENSE
 * file.
 *
 * See the [COPYING.md](../COPYING.md) file at the top-level directory of this
 * distribution and at https://bitbucket.org/cpslccauna/CpSignature/COPYING.md
 * for more legal information.
 */

#ifndef CVX_DESCRIPTOR_CPSIGNATURE_HPP__INCLUDED
#define CVX_DESCRIPTOR_CPSIGNATURE_HPP__INCLUDED

#pragma once

#include "CpsMatrix.hpp"
#include "DistanceL2.hpp"
#include <cassert>


namespace cvx {

    template< typename DistanceFunction >
    class CpSignature_ {
    public:

        typedef DistanceFunction distance_t; /**< The embedded metric distance. */

        CpSignature_() {
            // NOOP
        }

        ~CpSignature_() {
            // NOOP
        }

    public:

        CpsMatrix full_cps(const std::vector<cv::Point>& ctr, const unsigned M,
                           const unsigned N, const Real norm) {
            CV_Assert(M >= 3 and N >= 3);
            const unsigned k = sampling(M, N),
                    dm = k / M, dn = k / (N + 1);
            const Real inorm = (Real) 1 / norm;
            const distance_t dis;
            Matrix1r mtx(M, N);
            for (unsigned i = 0, m = 0; i < M; ++i, m += dm) {
                // m = 0, dm, 2dm, ...
                Real* p = mtx.ptr<Real>(i);
                for (unsigned j = 0, n = m + dn; j < N; ++j, n += dn) {
                    // n = m + dn, m + 2dn, ...
                    if (n >= k) {
                        n %= k;
                    }
                    p[j] = dis(ctr[m], ctr[n]) * inorm;
                }
            }
            return CpsMatrix(std::move(mtx));
        }

        /**
         * @brief       Returns a suitable contour sampling size for the
         *              required signature.
         * @param[in]   m Curve sampling size.
         * @param[in]   n Signature sampling size.
         * @return      An unsigned integer with the appropriate sampling size.
         */
        static unsigned sampling(const unsigned m, const unsigned n) {
            const Real U = m, V = n + 1;
            Real u = U, v = V, t;
            // Modified "Fast" Euclidean algorithm.
            do {
                t = v - (u * std::floor(v / u));
                v = u;
                u = t;
            } while (t != 0);
            // v : Greatest common divisor of U and V.
            // Returns the smallest common multiple of U and V.
            return static_cast< unsigned > (U * V / v);
        }

    };

    /**
     * @brief   Euclidean CPS functor.
     */
    typedef CpSignature_< DistanceEuclidean > CpsEuclidean;



} // namespace cvx


#endif // CVX_DESCRIPTOR_CPSIGNATURE_HPP__INCLUDED
