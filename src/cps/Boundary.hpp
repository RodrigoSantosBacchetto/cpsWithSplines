/**
 * @file        Boundary.hpp
 *
 * @brief       Defines a class of objects that holds information about the
 *              boundary of a planar point set.
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

#ifndef CVX_GEOMETRY_BOUNDARY_HPP__INCLUDED
#define CVX_GEOMETRY_BOUNDARY_HPP__INCLUDED

#pragma once

#include "Compatibility.hpp"
#include "Curve.hpp"
#include "Imports.hpp"
#include "Persistence.hpp"


// Forward declaration for class serialization mechanism.
CVX_DECLARE_CLASS_PERSISTENCE_PROXIES(Boundary)


namespace cvx {


    // Forward declarations for PlanarSet and ContourHierarchyNode classes.

    class PlanarSet;
    class ContourHierarchyNode;


    /**
     * @brief   A class of objects that hold the collection of curves
     *          representing the boundary of a planar point set.
     *
     * @details Objects of this class hold a collection (vector) of curves
     *          stored as sequences (vectors) of 2D points of class `Point`.
     *          These objects also store the hierarchy of the boundary curves.
     */
    class Boundary {

    public:

        // Types.

        typedef std::vector< Point > curve_t; /**< Curve as vector of points. */
        typedef std::vector< Vec4i > hierarchy_t; /**< Boundary hierarchy tree. */
        typedef ContourHierarchyNode adaptor_t; /**< Boundary hierarchy tree node adaptor. */
        typedef std::vector< adaptor_t > hadaptor_t; /**< Boundary hierarchy tree. */
        typedef std::vector< curve_t > boundary_t; /**< A collection of curves. */


    public:

        /**
         * @brief       Constructs an empty boundary.
         */
        Boundary() {
            // NOOP
        }


        /**
         * @brief       Constructs a copy of a given `Boundary` object.
         *
         * @param[in]   src  A constant reference to a `Boundary` object.
         */
        Boundary(const Boundary& src) :
        _boundary(src._boundary),
        _hierarchy(src._hierarchy) {
            // NOOP
        }


        /**
         * @brief       Constructs a `Boundary` by transferring data from a
         *              source `Boundary` object.
         *
         * @param[in]   src  A rvalue reference to a `Boundary` object.
         */
        Boundary(Boundary&& src) :
        _boundary(std::move(src._boundary)),
        _hierarchy(std::move(src._hierarchy)) {
            // NOOP
        }


        /**
         * @brief       Constructs a `Boundary` by transferring data from a
         *              source curve object.
         *
         * @param[in]   src  A rvalue reference to a `Curve` object.
         */
        Boundary(const Curve& curve) {
            _boundary.push_back(curve.data());
            _hierarchy.push_back(Vec4i(-1, -1, -1, -1));
        }


        /**
         * @brief       Destructor.
         */
        ~Boundary() {
            // NOOP
        }


        /**
         * @brief       Copies of a given `Boundary` object.
         *
         * @param[in]   src  A constant reference to a `Boundary` object.
         *
         * @return      A reference to this object.
         */
        Boundary& operator=(const Boundary& src) {
            return assign(src);
        }


        /**
         * @brief       Transfers data from a source `Boundary` object.
         *
         * @param[in]   src  A rvalue reference to a `Boundary` object.
         *
         * @return      A reference to this object.
         */
        Boundary& operator=(Boundary&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment. Copies of a given `Boundary` object.
         *
         * @param[in]   src  A constant reference to a `Boundary` object.
         *
         * @return      A reference to this object.
         */
        Boundary& assign(const Boundary& src) {
            _boundary = src._boundary;
            _hierarchy = src._hierarchy;
            return *this;
        }


        /**
         * @brief       Move assignment. Transfers data from a source
         *              `Boundary` object.
         *
         * @param[in]   src  A rvalue reference to a `Boundary` object.
         *
         * @return      A reference to this object.
         */
        Boundary& assign(Boundary&& src) {
            _boundary = std::move(src._boundary);
            _hierarchy = std::move(src._hierarchy);
            return *this;
        }


        /**
         * @brief       Extracts all boundary curves up to a given depth in the
         *              contours hierarchy tree.
         *
         * @param[in]   depth  Maximum depth to extract. Topmost level is 0.
         *
         * @return      A constant reference to a vector of indices
         *              representing the boundary hierarchy tree, an object of
         *              type `std::vector< Vec4i >`.
         */
        boundary_t cut(const int depth) const;


        /**
         * @brief       Accessor to the collection of boundary's curves.
         *
         * @return      A constant reference to a vector of curves.
         *              an object of type `std::vector< std::vector< Point > >`.
         */
        const boundary_t& data() const {
            return _boundary;
        }


        /**
         * @brief       Accessor to the i-th curve in the boundary.
         *
         * @return      A constant reference to a vector of 2D points,
         *              an object of type `std::vector< Point >`.
         */
        const curve_t& data(const unsigned i) const {
            return _boundary[i];
        }


        /**
         * @brief       Accessor to the hierarchy of curves in the boundary.
         *
         * @return      A constant reference to a vector of indices
         *              representing the boundary hierarchy tree, an object of
         *              type `std::vector< Hierarchy >`.
         */
        const hadaptor_t& hierarchy() const {
            return reinterpret_cast< const hadaptor_t& > (_hierarchy);
        }


        /**
         * @brief       Accessor to the i-th node un the hierarchy of curves.
         *
         * @return      A constant reference to a vector of indices
         *              representing the boundary hierarchy tree, an object of
         *              type `std::vector< Hierarchy >`.
         */
        const adaptor_t& hierarchy(const unsigned i) const {
            return reinterpret_cast< const adaptor_t& > (_hierarchy[i]);
        }


        /**
         * @brief       Get the first largest outermost boundary.
         *
         * @return      A vector of 2D points.
         */
        curve_t largest() const;


        /**
         * @brief       Extracts all boundary curves, if any, from a given depth
         *              level in the contours hierarchy tree.
         *
         * @param[in]   level  Depth to extract.
         *
         * @return      A constant reference to a vector of indices
         *              representing the boundary hierarchy tree, an object of
         *              type `std::vector< Vec4i >`.
         */
        boundary_t slice(const int level) const;


        /**
         * @brief       Reduce coordinates for displaying boundaries in a right
         *              handed coordinate system.
         *
         * @return      A `Boundary` object with the bundaries turned upside-down.
         */
        Boundary upsidedown() const;


    private:

        /**
         * @brief       Transfers data from a source objects.
         *
         * @param[in]   boundary  A rvalue reference to a collection of curves.
         *
         * @param[in]   hierarchy  A rvalue reference to a hierarchy of curves.
         *
         * @param[in]   roiRect  A rvalue reference to a `Rect` object.
         *
         * @param[in]   roiSize  A rvalue reference to a `Size` object.
         */
        Boundary(boundary_t&& boundary, hierarchy_t&& hierarchy) {
            set(std::move(boundary), std::move(hierarchy));
        }


        /**
         * @brief       Move setter.
         *
         * @param[in]   boundary  A rvalue reference to a collection of curves.
         *
         * @param[in]   hierarchy  A rvalue reference to a hierarchy of curves.
         *
         * @param[in]   roiRect  A rvalue reference to a `Rect` object.
         *
         * @param[in]   roiSize  A rvalue reference to a `Size` object.
         */
        void set(boundary_t&& boundary, hierarchy_t&& hierarchy) {
            _boundary = std::move(boundary);
            _hierarchy = std::move(hierarchy);
        }


        boundary_t _boundary; /**< The collection of boundary curves. */
        hierarchy_t _hierarchy; /**< The boundary hierarchy tree. */


        friend class PlanarSet;


        // Helpers for OpenCV serialization mechanism.
        static const char* const _node[5];


        enum node_t {
            CONTOURS_LIST,
            CONTOURS_TREE,
            CONTOUR_ITEM,
            NODE_ITEM,
            COLLECTION_SIZE,

        };


        CVX_DECLARE_CLASS_PERSISTENCE(Boundary, "boundary")

    };


} // namespace cvx


// Definitions for class serialization mechanism.
CVX_DEFINE_CLASS_PERSISTENCE_PROXIES(Boundary)


#endif // CVX_GEOMETRY_BOUNDARY_HPP__INCLUDED
