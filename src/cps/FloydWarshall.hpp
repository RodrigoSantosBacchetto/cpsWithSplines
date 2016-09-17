/* 
 * @file        FloydWarshall.hpp
 *
 * @brief       Defines an utility class for computing shortest-path distance
 *              using the Floyd-Warshall algorithm.
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

#ifndef CVX_ALGORITHM_FLOYD_WARSHALL_HPP__INCLUDED
#define CVX_ALGORITHM_FLOYD_WARSHALL_HPP__INCLUDED

#pragma once

#include "Imports.hpp"


namespace cvx {


    /**
     * @brief   Floyd-Warshall shortest-path distance utility class.
     */
    class FloydWarshall {

    public:

        /**
         * @brief       A type for the matrix of costs.
         */
        typedef Matrix1r matrix_t;


    public:

        void directed(matrix_t& matrix) {
            // All-pairs Floyd-Warshall will be faster in small dense graphs
            // than repeated Dijkstra's, or Bellman-Ford's, algorithms with
            // an adjacency matrix.
            //#pragma omp parallel for
            for (int k = 0; k < matrix.cols; ++k) {
                const Real* const vk = matrix.ptr<Real>(k);
                //#pragma omp parallel for firstprivate(k)
                for (int i = 0; i < k; ++i) {
                    Real* const vi = matrix.ptr<Real>(i);
                    //#pragma omp parallel for firstprivate(i)
                    for (int j = 0; j < i; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                    //#pragma omp parallel for firstprivate(i)
                    for (int j = i + 1; j < k; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                    //#pragma omp parallel for
                    for (int j = k + 1; j < matrix.cols; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                } // omp barrier
                //#pragma omp parallel for firstprivate(k)
                for (int i = k + 1; i < matrix.cols; ++i) {
                    Real* const vi = matrix.ptr<Real>(i);
                    //#pragma omp parallel for
                    for (int j = 0; j < k; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                    //#pragma omp parallel for firstprivate(i)
                    for (int j = k + 1; j < i; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                    //#pragma omp parallel for firstprivate(i)
                    for (int j = i + 1; j < matrix.cols; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                } // omp barrier
            } // omp barrier
        }


        void symmetric(matrix_t& matrix) {
            // All-pairs Floyd-Warshall will be faster in small dense graphs
            // than repeated Dijkstra's, or Bellman-Ford's, algorithms with
            // an adjacency matrix. This implementation optimze by exploting
            // the symmetric matrix and using only the triangular matrix.
            //#pragma omp parallel for
            for (int k = 0; k < matrix.cols; ++k) {
                const Real* vk = matrix.ptr<Real>(k);
                //#pragma omp parallel for firstprivate(k)
                for (int i = 0; i < k; ++i) {
                    Real* vi = matrix.ptr<Real>(i);
                    //#pragma omp parallel for firstprivate(i)
                    for (int j = 0; j < i; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                } // omp barrier
                //#pragma omp parallel for firstprivate(k)
                for (int i = k + 1; i < matrix.cols; ++i) {
                    Real* vi = matrix.ptr<Real>(i);
                    const Real vik = matrix(i, k);
                    //#pragma omp parallel for
                    for (int j = 0; j < k; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                    //#pragma omp parallel for firstprivate(i)
                    for (int j = k + 1; j < i; ++j) {
                        const Real vikj = vi[k] + vk[j];
                        if (vi[j] > vikj) {
                            vi[j] = vikj;
                        }
                    } // omp barrier
                } // omp barrier
            } // omp barrier
            completeSymm(matrix, true);
        }

    };


} // namespace cvx


#endif // CVX_ALGORITHM_FLOYD_WARSHALL_HPP__INCLUDED
