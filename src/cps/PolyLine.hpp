/* 
 * @file        PolyLine.hpp
 *
 * @brief       Defines a class of objects that holds information about the
 *              vertices of a crooked line, or subsampled (open) path.
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

#ifndef CVX_GEOMETRY_POLY_LINE_HPP__INCLUDED
#define CVX_GEOMETRY_POLY_LINE_HPP__INCLUDED

#pragma once

#include "PathBase.hpp"


// Forward declaration for class serialization mechanism.
CVX_DECLARE_CLASS_PERSISTENCE_PROXIES(PolyLine)


namespace cvx {


    /**
     * @brief   A class that represents a parameterized resampled contour
     *          function.
     */
    class PolyLine : PathBase_< int, false, false > {

    public:

        /**
         * @brief       Default constructor.
         */
        PolyLine() :
        PathBase_() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A constant reference to a source object.
         */
        PolyLine(const PolyLine& src) :
        PathBase_(src) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A rvalue reference to a source object.
         */
        PolyLine(PolyLine&& src) :
        PathBase_(std::move(src)) {
            // NOOP
        }


        /**
         * @brief       Copy wrapper constructor.
         *
         * @param[in]   data  A constant reference to a vector of points.
         */
        explicit
        PolyLine(const path_t& data) :
        PathBase_(data) {
            // NOOP
        }


        /**
         * @brief       Move wrapper constructor.
         *
         * @param[in]   data  A rvalue reference to a vector of points.
         */
        explicit
        PolyLine(path_t&& data) :
        PathBase_(std::move(data)) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~PolyLine() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        PolyLine& operator=(const PolyLine& src) {
            return (PathBase_::operator=(src), *this);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        PolyLine& operator=(PolyLine&& src) {
            return (PathBase_::operator=(std::move(src)), *this);
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        PolyLine& assign(const PolyLine& src) {
            return (PathBase_::assign(src), *this);
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        PolyLine& assign(PolyLine&& src) {
            return (PathBase_::assign(std::move(src)), *this);
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
         * @brief       Retrieves the point at a specific location on the path.
         *
         * @details     Search for a point on the path located at the given
         *              normalized arc-length. The arc-length parameter must
         *              be given in the valid range [0, 1].
         *
         * @remark      Since the path represent an open path, if the
         *              parameter is outside the valid range, an exception
         *              will be thrown. The valid range is [0, 1].
         *
         * @param[in]   t  The parameter, i.e. the normalized arc-lcength at
         *              which a path point should be retrieved. In the range
         *              [0, 1] for open paths.
         *
         * @return      Returns the point on the path located at the given
         *              normalized arc-length.
         */
        Point2r on(const Real t) const {
            // t in [0, 1].
            return get_open_on(t * length());
        }


    private:

        CVX_DECLARE_CLASS_PERSISTENCE(PolyLine, "crooked-line")

    };


} // namespace cvx


#endif // CVX_GEOMETRY_POLY_LINE_HPP__INCLUDED
