/**
 * @file        Distance.hpp
 *
 * @brief       Defines a generic normed distance functor class.
 *
 * @remark      This class template is the prototype, for all normed distance
 *              functors metaclasses and classes, i.e., it defines the
 *              interface for normed distance functors.
 *
 * @remark      Note that functors instantiated from these classes do not
 *              represent *metric* or *distance* functions, strictly speaking,
 *              in its rigorous mathematical definition, since they may not
 *              hold the triangle inequality, unless the embedded norm is a
 *              *true norm*. However, it is ensured, by design, that these
 *              distance functors are symmetric.
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

#ifndef CVX_METRIC_DISTANCE_HPP__INCLUDED
#define CVX_METRIC_DISTANCE_HPP__INCLUDED

#pragma once

#include "DistanceConnective.hpp"
#include "ConnectorFlat.hpp"


namespace cvx {


    /**
     * @tclass      DistanceConnective_< *, ConnectorFlat >
     *
     * @brief       Class template for generic normed distance functors.
     *
     * @details     Objects of classes instantiated from this class template
     *              computes the distance between two given points, using an
     *              embedded norm.
     *
     * @tparam      NormFunction  A class for norm functors.
     *
     * @remark      This class template is the prototype, for all distance
     *              functor metaclasses and classes, i.e., it defines the
     *              interface for distance functors and implement the interface
     *              for measure functors. Each instance of a distance functor
     *              class **must** implement the *rule of six*, additionally,
     *              **must** have copy and move constructor for norm functors,
     *              as well as copy and move `assignment()` members for
     *              distance functors, and copy and move setters, `set()`
     *              members, for norm functors. Also **must** overload the
     *              function call `operator()` for computing the distance
     *              between pairs of 2D points. The same for member function
     *              `embedded()`, which is added to be compatible with
     *              connectiveness checking distance functors.
     *
     * @remark      Special cases requiring additional information can have
     *              additional constructors, methods and data members as
     *              needed. The default constructor must initialize a default
     *              set of data members. If a case can't be applied without
     *              explicit initialization, the default constructor must
     *              throw an exception indicating the misuse of the functor.
     *
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     */
    template< typename NormFunction >
    class DistanceConnective_< NormFunction, ConnectorFlat > {

    public:

        /**
         * @brief      A type for norm functors.
         */
        typedef NormFunction norm_t;

        /**
         * @brief      A type for connection survey functors.
         */
        typedef ConnectorFlat connector_t;


    public:

        /**
         * @brief      Default constructor.
         */
        DistanceConnective_() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A connective distance functor.
         */
        DistanceConnective_(const DistanceConnective_& src) :
        norm(src.norm),
        connector(src.connector) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A connective distance functor.
         */
        DistanceConnective_(DistanceConnective_&& src) :
        norm(std::move(src.norm)),
        connector(std::move(src.connector)) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   nrm  A norm functor.
         */
        DistanceConnective_(const norm_t& nrm) :
        norm(nrm) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   nrm  A norm functor.
         */
        DistanceConnective_(norm_t&& nrm) :
        norm(std::move(nrm)) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   con  A connection survey functor.
         */
        DistanceConnective_(const connector_t& con) :
        connector(con) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   con  A connection survey functor.
         */
        DistanceConnective_(connector_t&& con) :
        connector(std::move(con)) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   nrm  A norm functor.
         *
         * @param[in]   con  A connection survey functor.
         */
        DistanceConnective_(const norm_t& nrm,
                            const connector_t& con) :
        norm(nrm),
        connector(con) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   nrm  A norm functor.
         *
         * @param[in]   con  A connection survey functor.
         */
        DistanceConnective_(norm_t&& nrm,
                            connector_t&& con) :
        norm(std::move(nrm)),
        connector(std::move(con)) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~DistanceConnective_() {
            // NOOP
        }


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
         * @return     True since this is a flat normed distance function.
         */
        template< typename Type >
        bool connected(const Point_< Type >& p1,
                       const Point_< Type >& p2) const {
            return true;
        }


        /**
         * @brief   Check if this is a conective measure.
         *
         * @remark  This is is a mandatory member function. It is part
         *          of the measure functor interface.
         *
         * @return  False since this is a flat normed distance function.
         */
        static bool connective() {
            return false;
        }


        /**
         * @brief   Check if this is a directed or asymmetric measure.
         *
         * @remark  This is is a mandatory member function. It is part
         *          of the measure functor interface.
         *
         * @return  False since this is a normed distance function.
         */
        static bool directed() {
            return false;
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
         * @brief       Call the embedded norm functor and compute
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
         */
        template< typename Type >
        Real operator()(const Point_< Type >& p1,
                        const Point_< Type >& p2) const {
            return embedded(p1, p2);
        }

    public:

        norm_t norm; /**< A embedded norm functor. */

        connector_t connector; /**< A embedded connector functor. */

    };


    /**
     * @tclass      Distance_< * >
     *
     * @brief       Class template for generic normed distance functors.
     *
     * @details     Objects of classes instantiated from this class template
     *              computes the distance between two given points, using an
     *              embedded norm.
     *
     * @tparam      NormFunction  A class for norm functors.
     *
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     */
    template< typename NormFunction >
    class Distance_ : public DistanceConnective_< NormFunction, ConnectorFlat > {


    public:


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A distance functor.
         *
         * @remark      This is a mandatory member function.
         */
        Distance_ operator=(const Distance_& src) {
            return assign(src);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A distance functor.
         *
         * @remark      This is a mandatory member function.
         */
        Distance_ operator=(Distance_&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A distance functor.
         *
         * @remark      This is a mandatory member function.
         */
        Distance_ assign(const Distance_& src) {
            return (DistanceConnective_< NormFunction, ConnectorFlat >::assign(src), *this);
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A distance functor.
         *
         * @remark      This is a mandatory member function.
         */
        Distance_ assign(Distance_&& src) {
            return (DistanceConnective_< NormFunction, ConnectorFlat >::assign(std::move(src)), *this);
        }

    };


} // namespace cvx


#endif // CVX_METRIC_DISTANCE_HPP__INCLUDED
