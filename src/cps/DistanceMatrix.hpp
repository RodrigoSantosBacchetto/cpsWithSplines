/**
 * @file        DistanceMatrix.hpp
 *
 * @brief       Defines a metaclass for nodes's weigthed adjacency matrix,
 *              also known as cost or distance matrix, computing classes
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

#ifndef CVX_ALGORITHM_DISTANCE_MATRIX_HPP__INCLUDED
#define	CVX_ALGORITHM_DISTANCE_MATRIX_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "PointSet.hpp"
#include "Polygon.hpp"


namespace cvx {


    template< typename DistanceFunction >
    class DistanceMatrix_ {

    public:

        /**
         * @brief       A type for the embedded cost/distance functor.
         */
        typedef DistanceFunction distance_t;


        /**
         * @brief       A type for the matrix of costs/distances.
         */
        typedef Matrix1r matrix_t;


    public:

        /**
         * @brief       Default constructor.
         */
        DistanceMatrix_() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A cost/distance matrix computing functor.
         */
        DistanceMatrix_(const DistanceMatrix_& src) :
        distance(src.distance) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A cost/distance matrix computing functor.
         */
        DistanceMatrix_(DistanceMatrix_&& src) :
        distance(std::move(src.distance)) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   dist  A cost/distance functor.
         */
        DistanceMatrix_(const distance_t& dist) :
        distance(dist) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   distance  A cost/distance functor.
         */
        DistanceMatrix_(distance_t&& dist) :
        distance(std::move(dist)) {
            // NOOP
        }


        /**
         * @brief       Destructor.
         */
        ~DistanceMatrix_() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A cost/distance matrix computing functor.
         */
        DistanceMatrix_& operator=(const DistanceMatrix_& src) {
            return assign(src);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A cost/distance matrix computing functor.
         */
        DistanceMatrix_& operator=(DistanceMatrix_&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A cost/distance matrix computing functor.
         */
        DistanceMatrix_& assign(const DistanceMatrix_& src) {
            return (distance = src.distance, *this);
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A cost/distance matrix computing functor.
         */
        DistanceMatrix_& assign(DistanceMatrix_&& src) {
            return (distance = std::move(src.distance), *this);
        }


        /**
         * @brief       Compute the distance matrix between all pairs of nodes
         *              (2D points) in a poit set.
         *
         * @param[in]   nodes  The set of nodes (points) of a sparse network.
         *
         * @return      A square array of floating-point values of type
         *              'Real' with the corresponding distances. It has
         *              many rows and columns as elements in the input set.
         */
        matrix_t operator()(const PointSet& pointset) const {
            // Search and compute the path distance matrix.
            matrix_t result(pointset.size(), pointset.size());
            if (distance_t::directed()) {
                // Compute the complete distance matrix, using
                // a directed or non-directed distance functor.
                distance_directed(result, pointset.data());
                if (distance_t::connective()) {
                    connect_directed(result, pointset.data());
                }
            } else {
                // Compute the complete distance matrix,
                // using a bidirectional distance functor.
                distance_symmetric(result, pointset.data());
                if (distance_t::connective()) {
                    connect_symmetric(result, pointset.data());
                }
            }
            return std::move(result);
        }


        /**
         * @brief       Compute the distance matrix between all pairs of nodes
         *              (2D points) in a polygonal network.
         *
         * @param[in]   polygon  The set of nodes (points) of a ring (polygon)
         *              network.
         *
         * @return      A square array of floating-point values of type
         *              'Real' with the corresponding distances. It has
         *              many rows and columns as elements in the input set.
         */
        matrix_t operator()(const Polygon& polygon) const {
            // Search and compute the path distance matrix. The
            // nodes are part of a ring, or closed circuit,
            // connect any pair of unconnnected nodes resulting
            // from discretization.
            matrix_t result(polygon.size(), polygon.size());
            if (distance_t::directed()) {
                // Compute the complete distance matrix, using
                // a directed or non-directed distance functor.
                distance_directed(result, polygon.data());
                if (distance_t::connective()) {
                    connect_directed(result, polygon.data());
                }
            } else {
                // Compute the complete distance matrix,
                // using a bidirectional distance functor.
                distance_symmetric(result, polygon.data());
                if (distance_t::connective()) {
                    connect_symmetric(result, polygon.data());
                }
            }
            return std::move(result);
        }


    private:

        template< typename Type >
        void distance_directed(matrix_t& matrix,
                               const std::vector< Point_< Type > >& nodes) const {
            // Compute the complete distance matrix, the distance of
            // each pair of nodes, using a directed, or unidirectional,
            // distance functor.
            const Point_< Type >* p = nodes.data();
            //#pragma omp parallel for
            for (int i = 0; (unsigned)i < nodes.size(); ++i) {
                Real* d = matrix.ptr<Real>(i);
                //#pragma omp parallel for firstprivate(d, i)
                for (int j = 0; j < i; ++j) {
                    d[j] = distance(p[i], p[j]);
                }
                d[i] = 0;
                //#pragma omp parallel for firstprivate(d, i)
                for (int j = i + 1; (unsigned)j < nodes.size(); ++j) {
                    d[j] = distance(p[i], p[j]);
                }
            }
        }


        template< typename Type >
        void distance_symmetric(matrix_t& matrix,
                                const std::vector< Point_< Type > >& nodes) const {
            // Compute the complete distance matrix using a
            // bidirectional distance functor.
            const Point_< Type >* p = nodes.data();
            //#pragma omp parallel for
            for (int i = 0; (unsigned)i < nodes.size(); ++i) {
                Real* d = matrix.ptr<Real>(i);
                //#pragma omp parallel for firstprivate(d, i)
                for (int j = 0; j < i; ++j) {
                    matrix(j, i) = d[j] = distance(p[i], p[j]);
                }
                d[i] = 0;
            }
        }


        template< typename Type >
        void connect_directed(matrix_t& matrix,
                              const std::vector< Point_< Type > >& nodes) const {
            // Connect contiguous nodes assuming they have consecutive
            // index numbers, i.e., two nodes having consecutive index
            // numbers are taken as contiguous nodes. Connect nodes
            // (1,2), (2,3), ..., (n-1,n), (n, 1).
            const Point_< Type >* p = nodes.data();
            //#pragma omp parallel for
            for (int j = 1; (unsigned)j < nodes.size(); ++j) {
                const int i = j - 1;
                // Connect nodes (i,i+1), (i+1, i).
                matrix(i, j) = distance.embedded(p[i], p[j]);
                matrix(j, i) = distance.embedded(p[j], p[i]);
            }
            // Connect nodes (n,1), (1, n).
            const int k = nodes.size() - 1;
            matrix(k, 0) = distance.embedded(p[k], p[0]);
            matrix(0, k) = distance.embedded(p[0], p[k]);
        }


        template< typename Type >
        void connect_symmetric(matrix_t& matrix,
                               const std::vector< Point_< Type > >& nodes) const {
            // Connect contiguous nodes assuming they have consecutive
            // index numbers, i.e., two nodes having consecutive index
            // numbers are taken as contiguous nodes. Connect nodes
            // (1,2), (2,3), ..., (n-1,n), (n, 1).
            const Point_< Type >* p = nodes.data();
            //#pragma omp parallel for
            for (int j = 1; (unsigned)j < nodes.size(); ++j) {
                // Connect nodes (i,i+1), (i+1,i).
                const int i = j - 1;
                matrix(j, i) =
                matrix(i, j) = distance.embedded(p[i], p[j]);
            }
            // Connect nodes (n,1), (1, n).
            const int k = nodes.size() - 1;
            matrix(0, k) =
            matrix(k, 0) = distance.embedded(p[k], p[0]);
        }

    public:

        distance_t distance;

    };


} // namespace cvx


#endif // CVX_ALGORITHM_DISTANCE_MATRIX_HPP__INCLUDED
