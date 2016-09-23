/**
 * @file        ConnectorFlat.hpp
 *
 * @brief       A flat connection survey functor class. The functor is a
 *              placeholder for 2D distance functors that does not require
 *              connectivity checking.
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

#ifndef CVX_METRIC_CONNECTOR_FLAT_HPP__INCLUDED
#define CVX_METRIC_CONNECTOR_FLAT_HPP__INCLUDED

#pragma once

#include "Imports.hpp"


namespace cvx {


    // Forward declarations for PlanarSet class.

    class PlanarSet;


    /**
     * @class       ConnectorFlat
     *
     * @brief       Class for flat-connection survey functors.
     *
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     */
    class ConnectorFlat {

    public:

        /**
         * @brief      A type for a 2D connectivity maps.
         */
        typedef Mat_< unsigned char > map_t;


    public:

        /**
         * @brief      Default constructor.
         */
        ConnectorFlat() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorFlat(const ConnectorFlat& src) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorFlat(ConnectorFlat&& src) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   map  A connectivity map mask.
         */
        ConnectorFlat(const PlanarSet& map) {
            // NOOP
        }


        /**
         * @brief       Normal constructor.
         *
         * @param[in]   map  A connectivity map mask.
         */
        ConnectorFlat(PlanarSet&& map) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~ConnectorFlat() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorFlat& operator=(const ConnectorFlat& src) {
            return *this;
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorFlat& operator=(ConnectorFlat&& src) {
            return *this;
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorFlat& assign(const ConnectorFlat& src) {
            return *this;
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A connective distance functor.
         */
        ConnectorFlat& assign(ConnectorFlat&& src) {
            return *this;
        }


        /**
         * @brief       Copy setter.
         *
         * @param[in]   map  A connectivity map mask.
         */
        void set(const PlanarSet& map) {
            // NOOP
        }


        /**
         * @brief       Move setter.
         * @param[in]   map  A connectivity map mask.
         */
        void set(PlanarSet&& map) {
            // NOOP
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
            return true;
        }

    };


} // namespace cvx


#endif // CVX_METRIC_CONNECTOR_FLAT_HPP__INCLUDED
