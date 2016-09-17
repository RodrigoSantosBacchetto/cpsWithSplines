/* 
 * @file        PathLength.hpp
 *
 * @brief       Defines a metaclass for classes of adaptor objects that hold
 *              information about the cumulative arc-length of the nodes of
 *              polyline paths.
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

#ifndef CVX_GEOMETRY_PATH_LENGTH_HPP__INCLUDED
#define CVX_GEOMETRY_PATH_LENGTH_HPP__INCLUDED

#include "Compatibility.hpp"
#include "Imports.hpp"
#include "Math.hpp"


namespace cvx {


    template< typename Type, bool ClosedPath, bool PixelNeighbors >
    class PathBase_;


    /**
     * @brief       Adaptor class for composite cumulative arc-length vector.
     */
    template< typename Type, bool PixelNeighbors >
    class PathLength_ {

    public:

        typedef Point_< Type > point_t;

        typedef Vector1r length_t; /**< A path length. */

        typedef std::vector< point_t > path_t; /**< A path of points. */


    private:

        PathLength_() :
        _perimeter(0) {
            // NOOP
        }


        PathLength_(const PathLength_& src) :
        _perimeter(src._perimeter),
        _data(src._data) {
            // NOOP
        }


        PathLength_(PathLength_&& src) :
        _perimeter(src._perimeter),
        _data(std::move(src._data)) {
            // NOOP
        }


        PathLength_& operator=(const PathLength_& src) {
            _perimeter = src._perimeter;
            _data = src._data;
            return *this;
        }


        PathLength_& operator=(PathLength_&& src) {
            _perimeter = src._perimeter;
            _data = std::move(src._data);
            return *this;
        }


    public:

        /**
         * @brief   Accessor to the path's length path.
         *
         * @return  A constant reference to the perimeter's length value.
         */
        const Real& operator()(/* NOARG */) const {
            return _perimeter;
        }


        /**
         * @brief       Returns the i-th cumulative arc-length.
         *
         * @param[in]   i  Index to the i-th cumulative arc-length, in the
         *              range [0, size()).
         *
         * @return      A real value with the i-th cumulative arc-length.
         */
        Real operator[](const unsigned i) const {
            return _data[i];
        }


        /**
         * @brief       Returns the i-th cumulative arc-length.
         *
         * @param[in]   i  Index to the i-th cumulative arc-length, in the
         *              range [0, size()).
         *
         * @return      A real value with the i-th cumulative arc-length.
         */
        Real at(const unsigned i) const {
            return _data.at(i);
        }


        /**
         * @brief   Accessor to the path's cumulative length path.
         *
         * @return  A constant reference to the length's path vector.
         */
        const length_t& data() const {
            return _data;
        }


        /**
         * @brief   Return the amount of samples in the length vector.
         *
         * @return  An unsigned integer with the amount of samples in the
         *          length vector.
         */
        size_t size() const {
            return _data.size();
        }

    private:

        void set(length_t&& length, const Real perimeter) {
            _perimeter = perimeter;
            _data = std::move(length);
        }


        /**
         * @brief       Update the cumulative arc-length path.
         *
         * @param[in]   closed  True if it is a closed path.
         *
         * @param[in]   norm  The norm used by the sampler.
         */
        void update(const path_t& path, const bool closed) {
            _data = length_t(path.size());
            _data[0] = 0;
#if defined(_OPENMP)
            //#pragma omp parallel for
            for (int j = 1; (unsigned)j < path.size(); ++j) {
                const int i = j - 1;
                _data[j] = _data[i] + distance(path[i], path[j]);
            }
#else
            for (unsigned i = 0, j = 1; j < path.size(); i = j++) {
                _data[j] = _data[i] + distance(path[i], path[j]);
            }
#endif // defined(_OPENMP)
            if (closed) {
                _perimeter = _data.back() + distance(path.back(), path.front());
            } else {
                _perimeter = _data.back();
            }
        }


        /**
         * @brief       Pixel neighborhood distance.
         *
         * @param[in]   p1  A constant reference to the first point.
         *
         * @param[in]   p2  A constant reference to the second point.
         *
         * @return      The discrete distance between neighbor pixels.
         */
        static
        Real distance(const point_t& p1, const point_t& p2) {
            return dist_l2(p1, p2);
        }


        Real _perimeter; /**< The path perimeter's length. */

        length_t _data; /**< The cumulative arc-length vector. */


        friend class PathBase_< Type, true, PixelNeighbors >;

        friend class PathBase_< Type, false, PixelNeighbors >;

    };


    /**
     * @brief       Specialization for neighbors pixels distance.
     */
    template<> inline
    Real PathLength_<int, true>::distance(const Point& p1, const Point& p2) {
        return (p1.x != p2.x and p1.y != p2.y ? CVX_SQRT2 : 1.0);
    }


} // namespace cvx


#endif // CVX_GEOMETRY_PATH_LENGTH_HPP__INCLUDED
