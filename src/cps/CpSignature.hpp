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
#include "Curve.hpp"
#include "DistanceMatrix.hpp"
#include "FloydWarshall.hpp"
#include "Metric.hpp"
#include "ShortestPathDistance.hpp"
#include <cassert>


namespace cvx {

    template< typename DistanceFunction >
    class CpSignature_ {
    public:

        typedef Matrix1r matrix_t;
        typedef DistanceFunction distance_t; /**< The embedded metric distance. */
        typedef DistanceMatrix_< distance_t > distance_matrix_functor_t;
        typedef ShortestPathDistance_< FloydWarshall, DistanceFunction >
                        shortest_distance_algorithm_t;

        CpSignature_() {
            // NOOP
        }

        ~CpSignature_() {
            // NOOP
        }

        /**
         * @brief       Compute the Contour-Point Signature, using a provided 2D
         *              distance functor, for M sampled points, given a contour
         *              and the signature definition level N.
         * @param[in]   ctr The given contour.
         * @param[in]   M Curve sampling size.
         * @param[in]   N Signature definition level.
         * @return      A (M by N) real matrix in which each row is the
         *              signature of a given point on the provided contour.
         */
        CpsMatrix operator()(const Curve& ctr, const unsigned M,
                             const unsigned N) {
            const Real area = std::sqrt(contourArea(ctr.data()));
            if (distance_t::directed()) {
                if (distance_t::connective()) {
                    return full_cps_mat(ctr, M, N, area);
                } else {
                    return full_cps(ctr, M, N, area);
                }
            } else {
                const unsigned P = N + 1;
                if (distance_t::connective()) {
                    if (M == P) {
                        return fast_cps_mat(ctr, M, area);
                    } else if (M % P == 0) {
                        return fast_cps_mat(ctr, M, N, area);
                    }
                    return full_cps_mat(ctr, M, N, area);
                } else {
                    if (M == P) {
                        return fast_cps(ctr, M, area);
                    } else if (M % P == 0) {
                        return fast_cps(ctr, M, N, area);
                    }
                    return full_cps(ctr, M, N, area);
                }
            }
        }

        /**
         * @brief       Compute the Contour-Point Signature, using a provided 2D
         *              distance functor, for M sampled points, given a contour
         *              and a signature level of definition equal to the contour
         *              sampling size minus one. This sampling size allows the
         *              use of a fast algorithm.
         * @param[in]   ctr The given contour.
         * @param[in]   M Curve sampling size.
         * @return      A (M by M-1) real matrix in which each row is the
         *              signature of a given point on the provided contour.
         */
        CpsMatrix operator()(const Curve& ctr, const unsigned M = 128) {
            const Real area = std::sqrt(contourArea(ctr.data()));
            if (distance_t::directed()) {
                if (distance_t::connective()) {
                    return full_cps_mat(ctr, M, M - 1, area);
                } else {
                    return full_cps(ctr, M, M - 1, area);
                }
            } else {
                if (distance_t::connective()) {
                    return fast_cps_mat(ctr, M, area);
                } else {
                    return fast_cps(ctr, M, area);
                }
            }
        }

        /**
         * @brief       Compute the Contour-Point Signature, using the full
         *              algorithn, provided a 2D distance functor, for M sampled
         *              points, given a contour sampled points, given a contour
         *              and the signature definition level N.
         * @param[in]   ctr The given contour.
         * @param[in]   M Curve sampling size.
         * @param[in]   N Signature definition level.
         * @return      A (M by N) real matrix in which each row is the
         *              signature of a given point on the provided contour.
         */
        CpsMatrix full(const Curve& ctr, const unsigned M, const unsigned N) {
            const Real area = std::sqrt(contourArea(ctr.data()));
            if (distance_t::connective()) {
                return full_cps_mat(ctr, M, N, area);
            } else {
                return full_cps(ctr, M, N, area);
            }
        }

    private:

        CpsMatrix full_cps(const Curve& ctr, const unsigned M,
                           const unsigned N, const Real norm) {
            CV_Assert(M >= 3 and N >= 3);
            const unsigned k = sampling(M, N),
                    dm = k / M, dn = k / (N + 1);
            const Real inorm = (Real) 1 / norm;
            const Polygon sc = ctr.resample(k);
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
                    p[j] = dis(sc[m], sc[n]) * inorm;
                }
            }
            return CpsMatrix(std::move(mtx));
        }

        CpsMatrix full_cps_mat(const Curve& ctr, const unsigned M,
                               const unsigned N, const Real norm) {
            CV_Assert(M >= 3 and N >= 3);
            const unsigned k = sampling(M, N),
                    dm = k / M, dn = k / (N + 1);
            const Real inorm = 1 / norm;
            const Polygon nodes = ctr.resample(k);
            const distance_matrix_functor_t dma(_dis);
            matrix_t dismat = dma(Polygon(nodes));
            shortest_distance_algorithm_t::inplace(dismat);
            Mat dis = dismat;
            Matrix1r mtx(M, N);
            for (unsigned i = 0, m = 0; i < M; ++i, m += dm) {
                // m = 0, dm, 2dm, ...
                for (unsigned j = 0, n = m + dn; j < N; ++j, n += dn) {
                    // n = m + dn, m + 2dn, ...
                    if (n >= k) {
                        n %= k;
                    }
                    mtx.at< Real >(i, j) = dis.at< Real >(m, n) * inorm;
                }
            }
            return CpsMatrix(std::move(mtx));
        }

        CpsMatrix fast_cps(const Curve& ctr, const unsigned M,
                           const unsigned N, const Real norm) {
            CV_Assert(M >= 8 and N >= 3);
            const unsigned P = N - 1, Q = N + 1;
            const unsigned di = M / Q, R = M - di;
            CV_Assert(M % Q == 0);
            const Real inorm = (Real) 1 / norm;
            const Polygon sc = ctr.resample(M);
            const distance_t dis;
            Matrix1r mtx(M, N);
            for (unsigned i = 0, l = 0; i < R; i += di, ++l) {
                const unsigned S = N - l;
                for (unsigned j = 0, n = i + di; j < S; ++j, n += di) {
                    const unsigned h = P - j;
                    for (unsigned k = 0; k < di; ++k) {
                        const unsigned a = i + k, g = n + k;
                        const Real val = dis(sc[a], sc[g]) * inorm;
                        mtx(a, j) = val;
                        mtx(g, h) = val;
                    }
                }
            }
            return CpsMatrix(std::move(mtx));
        }

        CpsMatrix fast_cps_mat(const Curve& ctr, const unsigned M,
                               const unsigned N, const Real norm) {
            CV_Assert(M >= 8 and N >= 3);
            const unsigned P = N - 1, Q = N + 1;
            const unsigned di = M / Q, R = M - di;
            CV_Assert(M % Q == 0);
            const Real inorm = 1 / norm;
            const Polygon nodes = ctr.resample(M);
            const distance_matrix_functor_t dm(_dis);
            matrix_t dismat = dm(Polygon(nodes));
            shortest_distance_algorithm_t::inplace(dismat);
            Matrix1r dis = dismat;
            Matrix1r mtx(M, N);
            for (unsigned i = 0, l = 0; i < R; i += di, ++l) {
                const unsigned S = N - l;
                for (unsigned j = 0, n = i + di; j < S; ++j, n += di) {
                    const unsigned h = P - j;
                    for (unsigned k = 0; k < di; ++k) {
                        const unsigned a = i + k, g = n + k;
                        const Real val = dis.at< Real >(a, g) * inorm;
                        mtx.at< Real >(a, j) = val;
                        mtx.at< Real >(g, h) = val;
                    }
                }
            }
            return CpsMatrix(std::move(mtx));
        }

        CpsMatrix fast_cps(const Curve& ctr, const unsigned M,
                           const Real norm) {
            CV_Assert(M >= 4);
            const Polygon sc = ctr.resample(M);
            const unsigned N = M - 1, P = M - 2;
            Mat mtx(M, N, CVX_FC(1));
            const Real inorm = 1 / norm;
            for (unsigned i = 0; i < N; ++i) {
                const unsigned Q = N - i;
                for (unsigned j = 0, n = i + 1; j < Q; ++j, ++n) {
                    const Real val = _dis(sc[i], sc[n]) * inorm;
                    mtx.at< Real >(i, j) = val;
                    mtx.at< Real >(n, P - j) = val;
                }
            }
            return CpsMatrix(std::move(mtx));
        }

        CpsMatrix fast_cps_mat(const Curve& ctr, const unsigned M,
                               const Real norm) {
            CV_Assert(M >= 4);
            const unsigned N = M - 1, P = M - 2;
            const Real inorm = 1 / norm;
            const Polygon nodes = ctr.resample(M);
            const distance_matrix_functor_t dm(_dis);
            matrix_t dismat = dm(Polygon(nodes));
            shortest_distance_algorithm_t::inplace(dismat);
            Mat dis = dismat;
            Mat mtx(M, N, CVX_FC(1));
            for (unsigned i = 0; i < N; ++i) {
                const unsigned Q = N - i;
                for (unsigned j = 0, n = i + 1; j < Q; ++j, ++n) {
                    const Real val = dis.at< Real >(i, n) * inorm;
                    mtx.at< Real >(i, j) = val;
                    mtx.at< Real >(n, P - j) = val;
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

    /**
     * @brief   Taxicab CPS functor.
     */
    typedef CpSignature_< DistanceManhattan > CpsManhattan;

    /**
     * @brief   Chebyshev CPS functor.
     */
    typedef CpSignature_< DistanceChebyshev > CpsChebyshev;

    /**
     * @brief   Inner topology geodesic distance CPS functor.
     */
    typedef CpSignature_< InnerDistanceEuclidean > CpsInnerDistance;


} // namespace cvx


#endif // CVX_DESCRIPTOR_CPSIGNATURE_HPP__INCLUDED
