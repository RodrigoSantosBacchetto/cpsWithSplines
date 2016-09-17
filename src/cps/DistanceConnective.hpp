/**
 * @file        DistanceConnective.hpp
 *
 * @brief       Defines generic connective distance functor class. A connective
 *              distance uses an internal map, or other mechanism, to determine
 *              the connectedness of two given points before computing the
 *              distance between them using a norm functor.
 *
 * @remark      This class template is the prototype, for all distance functors
 *              metaclasses and classes, i.e., it defines the interface for
 *              distance functors.
 *
 * @remark      Note that functors instantiated from these classes do not
 *              represent *distance* functions, strictly speaking, since they
 *              depend on the connectedness of points.
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

#ifndef CVX_METRIC_DISTANCE_CONNECTIVE_HPP__INCLUDED
#define CVX_METRIC_DISTANCE_CONNECTIVE_HPP__INCLUDED

#pragma once

#include "Imports.hpp"


namespace cvx {

    /**
     * @tclass      DistanceConnective_< * >
     *
     * @brief       Class template for generic connective normed distance
     *              functors.
     *
     * @details     Objects of classes instantiated from this class template
     *              computes the distance between two given points, using an
     *              embedded norm and a connectedness checking functor.
     *
     * @tparam      NormFunction  A class for norm functors.
     *
     * @tparam      Connector  A class for connection survey functors.
     *
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     */
    template< typename NormFunction, typename Connector >
    class DistanceConnective_ {

    public:

        /**
         * @brief      A type for norm functors.
         */
        typedef NormFunction norm_t;

        /**
         * @brief      A type for connection survey functors.
         */
        typedef Connector connector_t;


    public:


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A connective distance functor.
         */
        DistanceConnective_& operator=(const DistanceConnective_& src) {
            return assign(src);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A connective distance functor.
         */
        DistanceConnective_& operator=(DistanceConnective_&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A connective distance functor.
         */
        DistanceConnective_& assign(const DistanceConnective_& src) {
            norm = src.norm;
            connector = src.connector;
            return *this;
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A connective distance functor.
         */
        DistanceConnective_& assign(DistanceConnective_&& src) {
            norm = std::move(src.norm);
            connector = std::move(src.connector);
            return *this;
        }


        /**
         * @brief      Check if two 2D points are connected by a path.
         *
         * @param[in]  p1  A constant reference to the first point.
         *
         * @param[in]  p2  A constant reference to the second point.
         *
         * @return     True if point `p1` is connected to point `p2`
         *             by a path contained within the given map.
         */
        template< typename Type >
        bool connected(const Point_< Type >& p1,
                       const Point_< Type >& p2) const {
            return connector(p1, p2);
        }


        /**
         * @brief   Check if this is a conective measure.
         *
         * @remark  This is is a mandatory member function. It is part
         *          of the measure functor interface.
         *
         * @return  True since this is a connective distance function.
         */
        static bool connective() {
            return true;
        }


        /**
         * @brief   Check if this is a directed or asymmetric measure.
         *
         * @remark  This is is a mandatory member function. It is part
         *          of the measure functor interface.
         *
         * @return  True if this is a directed distance, false if it is
         *          symmetric.
         */
        static bool directed() {
            return connector_t::directed();
        }


        /**
         * @brief       Call the embedded norm functor and compute
         *              the norm of a given 2D point.
         *
         * @details     This member effectively computes the distance
         *              between two 2D points using the embedded metric
         *              functor.
         *
         * @remark      This is a mandatory member function.
         *
         * @param[in]   p  A constant reference to the point.
         *
         * @return      The norm of point `p`, a floating point value
         *              of type `Real`.
         */
        template< typename Type >
        Real embedded(const Point_< Type >& p) const {
            return norm(p);
        }


        /**
         * @brief       Call the embedded metric functor and compute
         *              the distance between two 2D points.
         *
         * @details     This member effectively computes the distance
         *              between two 2D points using the embedded metric
         *              functor.
         *
         * @remark      This is a mandatory member function.
         *
         * @param[in]   p1  A constant reference to the first point.
         *
         * @param[in]   p2  A constant reference to the second point.
         *
         * @return      The distance between points `p1` and `p2`, a
         *              floating point value of type `Real`.
         */
        template< typename Type >
        Real embedded(const Point_< Type >& p1,
                      const Point_< Type >& p2) const {
            return norm(p2 - p1);
        }


        /**
         * @brief       Compute the distance between two 2D points.
         *
         * @remark      This is a mandatory member function.
         *
         * @param[in]   p1  A constant reference to the first point.
         *
         * @param[in]   p2  A constant reference to the second point.
         *
         * @return      The distance between points `p1` and `p2`, a
         *              floating point value of type `Real`.
         *              If a path joining both points doesn't exist,
         *              `CVX_INFTY'  is returned.
         */
        template< typename Type >
        Real operator()(const Point_< Type >& p1,
                          const Point_< Type >& p2) const {
            return (connected(p1, p2) ? embedded(p1, p2) : CVX_INFTY);
        }


    public:

        norm_t norm; /**< A embedded norm functor. */

        connector_t connector; /**< A embedded connector functor. */

    };


} // namespace cvx


#endif // CVX_METRIC_DISTANCE_CONNECTIVE_HPP__INCLUDED
