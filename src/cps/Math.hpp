/**
 * @file        Math.hpp
 *
 * @brief       Common mathematical constants.
 *
 * @details     Defines basic mathematical constants common in older C/C++
 *              non standard libraries.
 *
 * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 *
 * @version     1.0
 *
 * @date        2015-03-20
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

#ifndef CVX_UTIL_MATH_HPP__INCLUDED
#define CVX_UTIL_MATH_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include <cmath> // Use template version of math routines (since C++11)
#include <limits> // Use std::numeric_limits utility class template.


// Fundamental constants
//
// Useful constants, similar to thoses M_* constants traditionally, or
// previously, defined in math.h. These constants were not part of the
// ISO/ANSI C standard.
//
#define CVX_E        2.7182818284590452353602874713527  // M_E        -> e^1
#define CVX_LOG2E    1.4426950408889634073599246810019  // M_LOG2E    -> log_2 e == 1 / ln(2)
#define CVX_LOG10E   0.43429448190325182765112891891661 // M_LOG10E   -> log_10 e == 1 / ln(10)
#define CVX_LOG2     0.69314718055994530941723212145818 // M_LN2      -> ln 2 === CV_LOG2
#define CVX_LN10     2.3025850929940456840179914546844  // M_LN10     -> ln 10
#define CVX_2PI      6.283185307179586476925286766559   // CV_2PI     -> 2 pi
#define CVX_PI       3.1415926535897932384626433832795  // CV_PI      -> pi
#define CVX_PI_2     1.5707963267948966192313216916398  // M_PI_2     -> pi / 2
#define CVX_PI_4     0.78539816339744830961566084581988 // M_PI_4     -> pi / 4
#define CVX_1_PI     0.31830988618379067153776752674503 // M_1_PI     -> 1 / pi
#define CVX_2_PI     0.63661977236758134307553505349006 // M_2_PI     -> 2 / pi
#define CVX_2_SQRTPI 1.1283791670955125738961589031215  // M_2_SQRTPI -> 2 / sqrt(pi)
#define CVX_SQRT2    1.4142135623730950488016887242097  // M_SQRT2    -> sqrt(2)
#define CVX_SQRT1_2  0.70710678118654752440084436210485 // M_SQRT1_2  -> sqrt(1/2)
#define CVX_INFTY_(Type) std::numeric_limits< Type >::infinity()


// Type dependent numeric constants.

#define CVX_INFTY32F CVX_INFTY_(cvx::Real4)
#define CVX_INFTY64F CVX_INFTY_(cvx::Real8)
#define CVX_INFTY CVX_INFTY_(cvx::Real)


/**
 * To avoid overflow or underflow at intermediate stages
 * of the computation, define CVX_MATH_USING_STD_HYPOT.
 */
#if defined(CVX_MATH_USING_STD_HYPOT)


namespace cvx {

    /**
     * Computes the hypotenuse of the given cathetus.
     */
    template< typename Type > inline
    Real hypot(const Real& x, const Real& y) {
        return std::hypot(x, y);
    }


    /**
     * Computes the euclidean norm of the given vector.
     */
    template< typename Type > inline
    Real norm_l2(const Point_< Type >& p) {
        return hypot(p.x, p.y);
    }


    /**
     * Computes the l2-norm induced distance.
     */
    template< typename Type > inline
    Real dist_l2(const Point_< Type >& p1, const Point_< Type >& p2) {
        return norm_l2(p2 - p1);
    }


    /**
     * Computes the length of the normal segment from point `c` to the
     * line passing through (or segment subtended by) points `a` and `b`.
     */
    template< typename Type > inline
    Real normal_l2(Point_< Type >& a, Point_< Type >& b,
                   Point_< Type >& c) {
        Point_< Type > d = b - a;
        Real n = std::max(std::abs(d.x), std::abs(d.y));
        n = std::max(std::abs(c.x), std::max(std::abs(c.y), n));
        n = std::max(std::abs(b.x), std::max(std::abs(b.y), n));
        n = std::max(std::abs(a.x), std::max(std::abs(a.y), n));
        d /= n; c /= n; b /= n; a /= n;
        const Real den = std::sqrt(d.x * d.x + d.y * d.y);
        const Real num = d.y * c.x - d.x * c.y + b.x * a.y - b.y * a.x;
        return n * std::abs(num) / den;
    }


    /**
     * Computes the euclidean norm of the given vector.
     */
    template< typename Type, unsigned int P > inline
    Real norm_lp(const Point_< Type >& p) {
        static const Real q = (Real) 1 / (Real) P;
        Real cx = std::abs(p.x);
        Real cy = std::abs(p.y);
        const Real n = std::max(cx, cy);
        cx = std::pow(cx / n, (Real) P);
        cy = std::pow(cy / n, (Real) P);
        return n * std::pow(cx + cy, q);
    }


} // namespace cvx


#else


namespace cvx {

    /**
     * Computes the hypotenuse of the given cathetus.
     */
    template< typename Type > inline
    Real hypot(const Real& x, const Real& y) {
        return std::sqrt(x * x + y * y);
    }


    /**
     * Computes the euclidean norm of the given vector.
     */
    template< typename Type > inline
    Real norm_l2(const Point_< Type >& p) {
        return cv::norm(p);
    }


    /**
     * Computes the l2-norm induced distance.
     */
    template< typename Type > inline
    Real dist_l2(const Point_< Type >& p1, const Point_< Type >& p2) {
        return norm_l2(p2 - p1);
    }


    /**
     * Computes the length of the normal segment from point `c` to the
     * line passing through (or segment subtended by) points `a` and `b`.
     */
    template< typename Type > inline
    Real normal_l2(const Point_< Type >& a, const Point_< Type >& b,
                   const Point_< Type >& c) {
        const Point_< Type > d = b - a;
        const Real den = norm_l2(d);
        const Real num = d.y * c.x - d.x * c.y + b.x * a.y - b.y * a.x;
        return std::abs(num) / den;
    }


    /**
     * Computes the euclidean norm of the given vector.
     */
    template< typename Type, unsigned int P > inline
    Real norm_lp(const Point_< Type >& p) {
        static const Real q = (Real) 1 / (Real) P;
        Real cx = std::abs(p.x);
        Real cy = std::abs(p.y);
        cx = std::pow(cx, (Real) P);
        cy = std::pow(cy, (Real) P);
        return std::pow(cx + cy, q);
    }


} // namespace cvx


#endif // defined(CVX_MATH_USING_STD_HYPOT)


#endif // CVX_UTIL_MATH_HPP__INCLUDED
