/**
 * @file        Curve.hpp
 *
 * @brief       Defines class of objects that hold information about a closed
 *              path line, e.g., a discrete curve or the vertices of a polygon.
 *
 * @brief       Defines a class that holds information about a (closed) curve,
 *              e.g., the contour of an object.
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

#ifndef CVX_GEOMETRY_CURVE_HPP__INCLUDED
#define CVX_GEOMETRY_CURVE_HPP__INCLUDED

#pragma once

#include "PathBase.hpp"
#include <cstdint>


// Forward declaration for class serialization mechanism.
CVX_DECLARE_CLASS_PERSISTENCE_PROXIES(Curve)


namespace cvx {

    /**
     * @brief   A class that represents a parameterized contour function.
     */
    class Curve : public PathBase_< int, true, true > {

    public:

        /**
         * @brief   Default constructor.
         */
        Curve() :
        PathBase_() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src A constant reference to a source object.
         */
        Curve(const Curve& src) :
        PathBase_(src) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src A rvalue reference to a source object.
         */
        Curve(Curve&& src) :
        PathBase_(std::move(src)) {
            // NOOP
        }


        /**
         * @brief       Copy wrapper constructor.
         *
         * @param[in]   data A constant reference to a contour vector.
         */
        Curve(const path_t& data) :
        PathBase_(data) {
            // NOOP
        }


        /**
         * @brief       Move wrapper constructor.
         *
         * @param[in]   data A rvalue reference to a contour vector.
         */
        Curve(path_t&& data) :
        PathBase_(std::move(data)) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~Curve() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        Curve& operator=(const Curve& src) {
            return (PathBase_::operator=(src), *this);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        Curve& operator=(Curve&& src) {
            return (PathBase_::operator=(std::move(src)), *this);
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        Curve& assign(const Curve& src) {
            return (PathBase_::assign(src), *this);
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        Curve& assign(Curve&& src) {
            return (PathBase_::assign(std::move(src)), *this);
        }


        /**
         * @brief       Retrieves the node nearest to a location on the path.
         *
         * @details     Search for a point on the path located nearest to
         *              the given normalized arc-length. The entire path is
         *              contained in the range [0, 1).
         *
         * @remark      Since the path represent a closed, or cyclic, path, if
         *              the parameter is outside the given range, the method
         *              will reduce its value into that range. That is, the
         *              valid range is (-∞, +∞).
         *
         * @param[in]   t  The parameter, i.e. the normalized arc-lcength at
         *              which a path point should be retrieved. In the range
         *              (-∞, +∞) for closed paths.
         *
         * @return      Returns the point on the path located nearest to the
         *              given normalized arc-length.
         */
        const Point& operator()(const Real t) const {
            // Reduce t in (-∞, +∞).
            Real r = t - (int64_t) t;
            if (r < 0) {
                r += 1;
            }
            return get_closed_at(r * length());
        }


        /**
         * @brief       Retrieves the point at a specific location on the path.
         *
         * @details     Search for a point on the path located at the given
         *              normalized arc-length. The entire path is contained
         *              in the range [0, 1).
         *
         * @remark      Since the path represent a closed, or cyclic, path, if
         *              the parameter is outside the given range, the method
         *              will reduce its value into that range. That is, the
         *              valid range is (-∞, +∞).
         *
         * @param[in]   t  The parameter, i.e. the normalized arc-lcength at
         *              which a path point should be retrieved. In the range
         *              (-∞, +∞) for closed paths.
         *
         * @return      Returns the point on the path located at the given
         *              normalized arc-length.
         */
        Point2r on(const Real t) const {
            // t in (-∞, +∞).
            Real r = t - (int64_t) t;
            if (r < 0) {
                r += 1;
            }
            return get_closed_on(r * length());
        }


        /**
         * @brief       Reads a contour file and (re)initializes this `Curve`
         *              object.
         *
         * @param[in]   fileName  The path and name of a contour image file.
         *
         * @remark      The function determines the type of an image by the
         *              content, not by the file extension.
         */
        void ctread(const std::string& fileName);


        /**
         * @brief       Saves a contour file and (re)initializes this `Curve`
         *              object.
         *
         * @param[in]   fileName  The path and name of a contour image file.
         *
         * @remark      The image format is chosen based on the filename
         *              extension:
         *              - `.c` Mokhtarian's contour data file.
         *              - `.ctr` Propietary contour data file.
         */
        bool ctwrite(const std::string& fileName) const;


        CVX_DECLARE_CLASS_PERSISTENCE(Curve, "curve")

    };


    /**
     * @brief           Implements OpenCV XML/YAML serialization.
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     * @param[in]       name  Reference to root element name.
     */
    inline
    void Curve::write(FileStorage& fs, const String& lbl) const {
        PathBase_::write(fs, lbl);
    }


    /**
     * @brief           Implements OpenCV XML/YAML deserialization.
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     */
    inline
    void Curve::read(const FileNode& fn, const Curve& obj) {
        PathBase_::read(fn, obj);
    }


} // namespace cvx


// Definitions for class serialization mechanism.
CVX_DEFINE_CLASS_PERSISTENCE_PROXIES(Curve)


#endif // CVX_GEOMETRY_CURVE_HPP__INCLUDED
