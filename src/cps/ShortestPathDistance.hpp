/**
 * @file        ShortestPathDistance.hpp
 *
 * @brief       Defines a metaclass for shortest path distance, or lower cost,
 *              matrix computing classes.
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

#ifndef CVX_ALGORITHM_SHORTEST_PATH_DISTANCE_HPP__INCLUDED
#define	CVX_ALGORITHM_SHORTEST_PATH_DISTANCE_HPP__INCLUDED

#pragma once

#include "Imports.hpp"


namespace cvx {


    template< typename  ShortingAlgorithm, typename DistanceFunction >
    class ShortestPathDistance_ {

    public:

        /**
         * @brief       A type for a shortest path algorithm traits.
         */
        typedef ShortingAlgorithm algorithm_t;

        /**
         * @brief       A type for a distance functor.
         */
        typedef DistanceFunction distance_t;

        /**
         * @brief       A type for the matrix of distances (or costs).
         */
        typedef Matrix1r matrix_t;


    public:

        ShortestPathDistance_() {
            // NOOP
        }


        ShortestPathDistance_(const ShortestPathDistance_& src) :
        algorithm(src.algorithm) {
            // NOOP
        }


        ShortestPathDistance_(ShortestPathDistance_&& src) :
        algorithm(std::move(src.algorithm)) {
            // NOOP
        }


        explicit
        ShortestPathDistance_(const algorithm_t& src) :
        algorithm(src) {
            // NOOP
        }


        explicit
        ShortestPathDistance_(algorithm_t&& src) :
        algorithm(std::move(src)) {
            // NOOP
        }


        ~ShortestPathDistance_() {
            // NOOP
        }


        ShortestPathDistance_& operator=(const ShortestPathDistance_& src) {
            return assign(src);
        }


        ShortestPathDistance_& operator=(ShortestPathDistance_&& src) {
            return assign(std::move(src));
        }


        ShortestPathDistance_& assign(const ShortestPathDistance_& src) {
            return (algorithm = src.algorithm, *this);
        }


        ShortestPathDistance_& assign(ShortestPathDistance_&& src) {
            return (algorithm = std::move(src.algorithm), *this);
        }


        /**
         * @brief       Compute the shortest distance matrix between all
         *              pairs of nodes (2D points) in a point set.
         *
         * @param[in]   matrix  The input distance matrix.
         *
         * @return      A matrix with the all-pair shortest distance.
         */
        matrix_t operator()(const matrix_t& matrix) const {
            // Search and compute the shortest
            // path distance matrix.
            matrix_t inmatrix = matrix.clone();
            inplace(inmatrix);
            return std::move(inmatrix);
        }


        /**
         * @brief       Compute the shortest distance matrix between all
         *              pairs of nodes (2D points) in a point set, in-place.
         *
         * @param[in,out]   matrix  The input distance matrix.
         */
        void inplace(matrix_t& matrix) {
            // Search and compute the shortest
            // path distance matrix.
            if (distance_t::directed()) {
                algorithm.directed(matrix);
            } else if (distance_t::connective()) {
                algorithm.symmetric(matrix);
            }
        }


    public:

        algorithm_t algorithm;

    };


} // namespace cvx


#endif // CVX_ALGORITHM_SHORTEST_PATH_DISTANCE_HPP__INCLUDED
