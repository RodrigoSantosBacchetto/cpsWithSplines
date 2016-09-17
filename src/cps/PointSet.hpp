/**
 * @file        PointSet.hpp
 *
 * @brief       Defines a class of objects that represent sparse planar point
 *              sets.
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

#ifndef CVX_GEOMETRY_POINT_SET_HPP__INCLUDED
#define CVX_GEOMETRY_POINT_SET_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Persistence.hpp"


// Forward declaration for class serialization mechanism.
CVX_DECLARE_CLASS_PERSISTENCE_PROXIES(PointSet)


namespace cvx {


    /**
     * @brief   A class that represents a sparse point set.
     */
    class PointSet {

    public:

        // Types.

        typedef int value_t; /**< A basic numeric type. */
        typedef Point_< value_t > point_t; /**< A point type. */
        typedef std::vector< point_t > set_t; /**< Sparse set as vector of points. */
        typedef Point2d centroid_t; /**< Centroid as a floating-point 2D point. */


    public:

        /**
         * @brief       Default constructor.
         */
        PointSet() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A constant reference to a source object.
         */
        PointSet(const PointSet& src) :
        _set(src._set) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A rvalue reference to a source object.
         */
        PointSet(PointSet&& src) :
        _set(std::move(src._set)) {
            // NOOP
        }


        /**
         * @brief       Copy wrapper constructor.
         *
         * @param[in]   data  A constant reference to a contour vector.
         */
        explicit
        PointSet(const set_t& data) :
        _set(data) {
            // NOOP
        }


        /**
         * @brief       Move wrapper constructor.
         *
         * @param[in]   data  A rvalue reference to a contour vector.
         */
        explicit
        PointSet(set_t&& data) :
        _set(std::move(data)) {
            // NOOP
        }


        /**
         * @brief       Destructor.
         */
        ~PointSet() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        PointSet& operator=(const PointSet& src) {
            return assign(src);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        PointSet& operator=(PointSet&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        PointSet& assign(const PointSet& src) {
            return (_set = src._set, *this);
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        PointSet& assign(PointSet&& src) {
            return (_set = std::move(src._set), *this);
        }


        /**
         * @brief       Copy setter.
         *
         * @param[in]   data  A constant reference to a contour vector.
         */
        void set(const set_t& data) {
            _set = data;
        }


        /**
         * @brief       Move setter.
         *
         * @param[in]   src  A rvalue reference to a source object.
         */
        void set(set_t&& data) {
            _set = std::move(data);
        }


        /**
         * @brief       Retrieves the i-th point.
         *
         * @return      Returns the i-th point in the set.
         */
        const Point& operator[](const unsigned i) const {
            return _set[i];
        }


        /**
         * @brief       Retrieves the i-th point.
         *
         * @return      Returns the i-th point in the set.
         */
        const Point& operator()(const unsigned i) const {
            return operator[](i);
        }


        /**
         * @brief       Return the amount of points in the set.
         *
         * @return      An unsigned integer with the amount of points
         *              in the set.
         */
        size_t size() const {
            return _set.size();
        }


        /**
         * @brief       Accessor to the set data.
         *
         * @return      A constant reference to the data vector.
         */
        const set_t& data() const {
            return _set;
        }


    private:

        set_t _set; /**< The sparse set of points. */


        // Helpers for OpenCV serialization mechanism.
        static const char* const _node[2];


        enum node_t {
            SIZE,
            POINTS
        };


        CVX_DECLARE_CLASS_PERSISTENCE(PointSet, "point-set")

    };


} // namespace cvx


// Definitions for class serialization mechanism.
CVX_DEFINE_CLASS_PERSISTENCE_PROXIES(PointSet)


#endif // CVX_GEOMETRY_POINT_SET_HPP__INCLUDED
