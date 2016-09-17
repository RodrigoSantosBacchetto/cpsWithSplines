/* 
 * @file        PathBase.hpp
 *
 * @brief       Defines a base class for objects that hold information about
 *              a closed, or open, path line, e.g., the nodes of a discrete
 *              curve or arc, or the vertices of a polygon or crooked line.
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

#ifndef CVX_GEOMETRY_PATH_BASE_HPP__INCLUDED
#define CVX_GEOMETRY_PATH_BASE_HPP__INCLUDED

#pragma once

#include "Compatibility.hpp"
#include "FreemanChain.hpp"
#include "Imports.hpp"
#include "Math.hpp"
#include "PathLength.hpp"
#include "Persistence.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <cassert>


namespace cvx {


    /**
     * @brief   A class that represents a parameterized path function.
     */
    template< typename Type, bool ClosedPath, bool PixelNeighbors >
    class PathBase_ {

    public:

        typedef Type value_t; /**< A basic numeric type. */
        typedef Point_< value_t > point_t; /**< A point type. */
        typedef std::vector< point_t > path_t; /**< A path of points. */
        typedef std::vector< Real > length_t; /**< A path length. */
        typedef PathLength_< Type, PixelNeighbors > length_adaptor;


    protected:

        /**
         * @brief   Default constructor.
         */
        PathBase_() :
        length(_length) {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         *
         * @param[in]   src  A constant reference to a source object.
         */
        PathBase_(const PathBase_& src) :
        length(_length),
        _data(src._data),
        _length(src._length) {
            // NOOP
        }


        /**
         * @brief       Move constructor.
         *
         * @param[in]   src  A rvalue reference to a source object.
         */
        PathBase_(PathBase_&& src) :
        length(_length),
        _data(std::move(src._data)),
        _length(std::move(src._length)) {
            // NOOP
        }


        /**
         * @brief       Copy wrapper constructor.
         *
         * @param[in]   path  A constant reference to a path vector.
         */
        PathBase_(const path_t& path) :
        length(_length),
        _data(path) {
            length.update(path, ClosedPath);
        }


        /**
         * @brief       Move wrapper constructor.
         *
         * @param[in]   path  A constant reference to a path vector.
         *
         * @param[in]   norm  The norm used by resampler.
         */
        PathBase_(path_t&& path) :
        length(_length),
        _data(std::move(path)) {
            length.update(_data, ClosedPath);
        }


        /**
         * @brief      Destructor.
         */
        ~PathBase_() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         *
         * @param[in]   src  A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        PathBase_& operator=(const PathBase_& src) {
            return assign(src);
        }


        /**
         * @brief       Move assignment operator.
         *
         * @param[in]   src  A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        PathBase_& operator=(PathBase_&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment.
         *
         * @param[in]   src  A constant reference to the source object.
         *
         * @return      A reference to this object.
         */
        PathBase_& assign(const PathBase_& src) {
            _data = src._data;
            _length = src._length;
            return *this;
        }


        /**
         * @brief       Move assignment.
         *
         * @param[in]   src  A rvalue reference to the source object.
         *
         * @return      A reference to this object.
         */
        PathBase_& assign(PathBase_&& src) {
            _data = std::move(src._data);
            _length = std::move(src._length);
            return *this;
        }


        /**
         * @brief       Move setter.
         *
         * @param[in]   path  A rvalue reference to a path vector.
         *
         * @param[in]   length  A rvalue reference to a length vector.
         *
         * @param[in]   centroid  The cetroid point.
         *
         * @param[in]   perimeter  The perimeter length.
         */
        void set(path_t&& path, length_t&& length, const Real perimeter) {
            CV_Assert(path.size() == length.size());
            _data = std::move(path);
            _length.set(std::move(length), perimeter);
        }


        /**
         * @brief       Retrieves the point nearest to a location in the path.
         *
         * @details     Search for a point on the path located nearest to the
         *              given normalized arc-length. If the parameter is
         *              outside the valid range, an exception will be thrown.
         *
         * @param[in]   s  The parameter, i.e., the arc-lcength at which a path
         *              point should be retrieved. In the range [0, length).
         *
         * @return      Returns the point on the path located nearest to the
         *              given arc-length. Note that this point is actually in
         *              the path collection, i.e., a stored node.
         */
        const point_t& get_closed_at(const Real s) const {
            CV_Assert(size() > 0 and length() > 0 // non-empty path.
                and size() == length.size()    // data coherence.
                and 0 <= s and s < length());  // s in [0, length()).
            // Find the lower bound.
            unsigned last = size();
            for (unsigned i = 0, j = 1; j < last; i = j++) {
                if (s <= length[j]) {
                    if (s - length[i] < length[j] - s) {
                        return _data[i];
                    } else {
                        return _data[j];
                    }
                }
            }
            if (s - length[--last] < length() - s) {
                return _data[last];
            }
            return _data[0];
        }


        /**
         * @brief       Retrieves the point nearest to a location in the path.
         *
         * @details     Search for a point on the path located nearest to the
         *              given normalized arc-length. If the parameter is
         *              outside the valid range, an exception will be thrown.
         *
         * @param[in]   s  The parameter, i.e., the arc-lcength at which a path
         *              point should be retrieved. In the range [0, length].
         *
         * @return      Returns the point on the path located nearest to the
         *              given arc-length. Note that this point is actually in
         *              the path collection, i.e., a stored node.
         */
        const point_t& get_open_at(const Real s) const {
            CV_Assert(size() > 0 and length() > 0 // non-empty path.
                and size() == length.size()    // data coherence.
                and 0 <= s and s <= length()); // s in [0, length()].
            if (s < 0 or s > length()) {
                throw std::range_error("Parameter outside the valid range.");
            }
            // Find the lower bound.
            unsigned last = size();
            for (unsigned i = 0, j = 1; j < last; i = j++) {
                if (s <= length[j]) {
                    if (s - length[i] < length[j] - s) {
                        return _data[i];
                    } else {
                        return _data[j];
                    }
                }
            }
            // Must never happen!
            throw std::domain_error("Unbounded parameter error.");
        }


        /**
         * @brief       Retrieves the point at a specific location on the path.
         *
         * @details     Search for a point on the path located exactly at the
         *              given normalized arc-length.
         *
         * @param[in]   s  The parameter, i.e., the arc-lcength at which a path
         *              point should be retrieved. In the range [0, length).
         *
         * @return      Returns the point on the path located at a specific
         *              location. Note that this point may not actually be in
         *              the path's node collection, i.e., it may be an
         *              interpolated point not a stored node.
         */
        const Point2r& get_closed_on(const Real s) const {
            CV_Assert(size() > 0 and length() > 0 // non-empty path.
                and size() == length.size()    // data coherence.
                and 0 <= s and s < length());  // s in [0, length()).
            // Find the lower bound.
            unsigned last = size();
            for (unsigned i = 0, j = 1; j < last; i = j++) {
                if (s <= length[j]) {
                    const Point2r r = _data[j] - _data[i];
                    return Point2r(_data[i]) + (s - length[i]) * r;
                }
            }
            const Point2r r = _data[0] - _data[--last];
            return Point2r(_data[last]) + (s - length[last]) * r;
        }


        /**
         * @brief       Retrieves the point at a specific location on the path.
         *
         * @details     Search for a point on the path located exactly at the
         *              given normalized arc-length.
         *
         * @param[in]   s  The parameter, i.e., the arc-lcength at which a path
         *              point should be retrieved. In the range [0, length].
         *
         * @return      Returns the point on the path located at a specific
         *              location. Note that this point may not actually be in
         *              the path's node collection, i.e., it may be an
         *              interpolated point not a stored node.
         */
        const Point2r& get_open_on(const Real s) const {
            CV_Assert(size() > 0 and length() > 0 // non-empty path.
                and size() == length.size()    // data coherence.
                and 0 <= s and s <= length()); // s in [0, length()].
            if (s < 0 or s > length()) {
                throw std::range_error("Parameter outside the valid range.");
            }
            // Find the lower bound.
            unsigned last = size();
            for (unsigned i = 0, j = 1; j < last; i = j++) {
                if (s <= length[j]) {
                    const Point2r r = _data[j] - _data[i];
                    return Point2r(_data[i]) + (s - length[i]) * r;
                }
            }
            // Must never happen!
            throw std::domain_error("Unbounded parameter error.");
        }


        /**
         * @brief           Implements OpenCV XML/YAML serialization.
         * @param[in,out]   fs  Reference to a `FileStorage` object.
         * @param[in]       name  Reference to root element name.
         */
        void write(FileStorage& fs, const String&) const {
            // Serialize contour data.
            fs << "{"
                    << _node[PERIMETER] << length()
                    << _node[VERTICES] << (int) size()
                    << _node[ARCLENGTH] << length.data()
                    << _node[PATH] << data()
                    << "}";
        }


        /**
         * @brief           Implements OpenCV XML/YAML deserialization.
         * @param[in,out]   fs  Reference to a `FileStorage` object.
         */
        void read(const FileNode& fn, const PathBase_&) {
            // Deserialize the contour data.
            const Real perimeter = (Real) fn[_node[PERIMETER]];
            const int vertices = (int) fn[_node[VERTICES]];
            length_t length;
            fn[_node[ARCLENGTH]] >> length;
            path_t nodes;
            fn[_node[PATH]] >> nodes;
            if (vertices != nodes.size() or vertices != length.size()) {
                throw std::logic_error("Corrupt path data");
            }
            set(std::move(nodes), std::move(length), perimeter);
        }


    public:

        /**
         * @brief       Indexing operator. Returns the i-th point on the path,
         *              if any.
         *
         * @param[in]   i  Index to the i-th point on the path, in the range
         *              [0, size()).
         *
         * @return      A constant reference to the i-th 2D point on the path.
         */
        const point_t& operator[](const unsigned i) const {
            return _data[i];
        }


        /**
         * @brief       Returns the i-th point on the path, if any.
         *
         * @param[in]   i  Index to the i-th point on the path, in the range
         *              [0, size()).
         *
         * @return      A constant reference to the i-th 2D point on the path.
         */
        const point_t& at(const unsigned i) const {
            return _data.at(i);
        }


        /**
         * @brief   Accessor to the path data.
         *
         * @return  A constant reference to the path's data vector.
         */
        const path_t& data() const {
            return _data;
        }


        /**
         * @brief   Return the amount of samples in the path.
         *
         * @return  An unsigned integer with the amount of samples in the
         *          path.
         */
        size_t size() const {
            return _data.size();
        }


        length_adaptor& length; /**< The cumulative arc-length vector. */


    private:

        path_t _data; /**< The path as a collection of nodes. */
        length_adaptor _length; /**< The cumulative arc-length vector. */


        // Helpers for OpenCV serialization mechanism.
        static const char* const _node[5];


        enum node_t {
            PERIMETER,
            VERTICES,
            ARCLENGTH,
            CHAINCODES,
            PATH
        };

    };


    template< typename Type, bool ClosedPath, bool PixelNeighbors >
    const char* const PathBase_< Type, ClosedPath, PixelNeighbors >::_node[5] = {
        "perimeter",
        "vertices",
        "arc-length",
        "chain-codes",
        "path"
    };


    /**
     * @brief           Implements OpenCV XML/YAML serialization.
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     * @param[in]       name  Reference to root element name.
     */
    template<> inline
    void PathBase_< int, false, true >::write(FileStorage& fs,
                                              const String&) const {
        // Get the Freeman chain-code fr this contour.
        const FreemanChain::chain_t chain = FreemanChain::encode(data());
        // Serialize contour data.
        fs << "{"
           << _node[PERIMETER] << length()
           << _node[VERTICES] << (int) size()
           << _node[ARCLENGTH] << length.data()
           << _node[CHAINCODES] << chain
           << "}";
    }


    /**
     * @brief           Implements OpenCV XML/YAML serialization.
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     * @param[in]       name  Reference to root element name.
     */
    template<> inline
    void PathBase_< int, true, true >::write(FileStorage& fs,
                                             const String&) const {
        // Get the Freeman chain-code fr this contour.
        const FreemanChain::chain_t chain = FreemanChain::encode(data());
        // Serialize contour data.
        fs << "{"
           << _node[PERIMETER] << length()
           << _node[VERTICES] << (int) size()
           << _node[ARCLENGTH] << length.data()
           << _node[CHAINCODES] << chain
           << "}";
    }


    /**
     * @brief           Implements OpenCV XML/YAML deserialization.
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     */
    template<>
    void PathBase_< int, false, true >::read(const FileNode& fn,
                                             const PathBase_&) {
        // Deserialize the contour data.
        const Real perimeter = (Real) fn[_node[PERIMETER]];
        const int vertices = (int) fn[_node[VERTICES]];
        length_t length;
        fn[_node[ARCLENGTH]] >> length;
        Point2r centroid;
        FreemanChain::chain_t chain;
        fn[_node[CHAINCODES]] >> chain;
        path_t nodes = FreemanChain::decode(chain);
        if (vertices != nodes.size() or vertices != length.size()) {
            throw std::logic_error("Corrupt path data");
        }
        set(std::move(nodes), std::move(length), perimeter);
    }


    /**
     * @brief           Implements OpenCV XML/YAML deserialization.
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     */
    template<>
    void PathBase_< int, true, true >::read(const FileNode& fn,
                                            const PathBase_&) {
        // Deserialize the contour data.
        const Real perimeter = (Real) fn[_node[PERIMETER]];
        const int vertices = (int) fn[_node[VERTICES]];
        length_t length;
        fn[_node[ARCLENGTH]] >> length;
        Point2r centroid;
        FreemanChain::chain_t chain;
        fn[_node[CHAINCODES]] >> chain;
        path_t nodes = FreemanChain::decode(chain);
        if (vertices != nodes.size() or vertices != length.size()) {
            throw std::logic_error("Corrupt path data");
        }
        set(std::move(nodes), std::move(length), perimeter);
    }


} // namespace cvx


#endif // CVX_GEOMETRY_PATH_BASE_HPP__INCLUDED
