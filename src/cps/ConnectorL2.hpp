/**
 * @file        ConnectorL2.hpp
 *
 * @brief       A connection survey functor class. The functor determines if
 *              two given 2D points are connected by a straight line lying
 *              within a point set defined with a provided planar set.
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

#ifndef CVX_METRIC_CONNECTOR_L2_HPP__INCLUDED
#define CVX_METRIC_CONNECTOR_L2_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "PlanarSet.hpp"


namespace cvx {

    /**
     * @class       ConnectorL2
     *
     * @brief       Class for euclidean-connection survey functors.
     *
     * @details     Objects of classes instantiated from this class
     *              act as functions that check if two 2D/3D points
     *              are connected by a straight line contained in
     *              point set defined in a user provided map.
     *
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     *
     * @date        2015-03-19
     */
    class ConnectorL2 {

    public:

        /**
         * @brief      A type for a 2D connectivity maps.
         */
        typedef PlanarSet::frame_t map_t;


    public:

        /**
         * @brief      Default constructor.
         */
        ConnectorL2() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorL2(const ConnectorL2& src) :
        _map(src._map) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorL2(ConnectorL2&& src) :
        _map(std::move(src._map)) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   map  A connectivity map mask.
         */
        ConnectorL2(const PlanarSet& map) :
        _map(map.data()) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   map  A connectivity map mask.
         */
        ConnectorL2(PlanarSet&& map) :
        _map(std::move(map._frame)) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~ConnectorL2() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorL2& operator=(const ConnectorL2& src) {
            return assign(src);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorL2& operator=(ConnectorL2&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorL2& assign(const ConnectorL2& src) {
            return (_map = src._map, *this);
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorL2& assign(ConnectorL2&& src) {
            return (_map = std::move(src._map), *this);
        }


        /**
         * @brief       Copy setter.
         *
         * @param[in]   map  A connectivity map mask.
         */
        ConnectorL2& set(const PlanarSet& map) {
            return (_map = map.data(), *this);
        }


        /**
         * @brief       Move setter.
         * @param[in]   map  A connectivity map mask.
         */
        ConnectorL2& set(PlanarSet&& map) {
            return (_map = std::move(map._frame), *this);
        }


        /**
         * @brief   Check if directed or asymmetric connection is supported.
         *
         * @remark  In a directed connection, the existence of a path from
         *          point `P` to point `Q` does not guarantees the existence
         *          of the reciprocal path from `Q` to `P`.
         *
         * @return  True if this supports directed distance, false if it is
         *          symmetric. For euclidean-connection it always returns
         *          false.
         */
        static bool directed() {
            return false;
        }


        /**
         * @brief       Verify if two 2D points are connected by a straight
         *              line residing within point set defined in the user
         *              provided map.
         *
         * @param[in]   p1  A constant reference to the first point.
         *
         * @param[in]   p2  A constant reference to the second point.
         *
         * @return      True if `p1` and `p2` are connected by a path, false
         *              otherwise.
         */
        template< typename Type >
        bool operator()(const Point_< Type >& p1,
                        const Point_< Type >& p2) const {
            return survey(p1, p2);
        }


    private:

        template< typename Point >
        bool survey(const Point& p1, const Point& p2) const {
            // Test connectivity two way since we are working
            // with a discrete set of cartesian points.
            return test(p1, p2) or test(p2, p1);
        }


        template< typename Type >
        bool test(const Point_< Type >& p1,
                  const Point_< Type >& p2) const {
            // Check if a straight line between p1 and p2,
            // lying in the point set defined in _map, exists.
            // _map contains a binary map, points outside the
            // set are assigned to zero.
            const Point_< Type > r = p2 - p1;
            const Real n = std::max(std::abs(r.x),
                                      std::abs(r.y));
            const Point2r s = Point2r(r) * (1 / n);
            bool connected = true;
            // Traverse the straight line between p1 and p2.
            Point2r p = p1;
            for (int k = 0; connected and k < n; ++k) {
                p += s;
                // Check if point p is in the point set.
                if (_map(Point(p)) == 0) {
                    // Return false if the line goes
                    // outside the point set.
                    connected = false;
                }
            }
            return connected;
        }


    private:

        map_t _map; /**< A binary map. */

    };


} // namespace cvx


#endif // CVX_METRIC_CONNECTOR_L2_HPP__INCLUDED
