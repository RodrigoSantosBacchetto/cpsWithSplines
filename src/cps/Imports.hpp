/**
 * @file        Imports.hpp
 *
 * @brief       Common types and imports from OpenCV.
 *
 * @details     Imports types and routines from the OpenCV library, and sets a
 *              compatibility layer with that library.
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

#ifndef CVX_UTIL_IMPORTS_HPP__INCLUDED
#define CVX_UTIL_IMPORTS_HPP__INCLUDED

#pragma once

#include <opencv2/core.hpp> // Types defined in OpenCV's core.
#include <vector>


#if !defined(CVX_DEFAULT_FLOAT)

/**
 * @brief  Default floating point type flag for this library.
 * @details The user may define the default floating point type for this
 *          library, by setting one of the OpenCV floating-point type flags
 *          CV_32F or CV_64F (default) before including these header files
 *          or in their project settings.
 */
#define CVX_DEFAULT_FLOAT CV_64F

#endif // !defined(CVX_DEFAULT_FLOAT)


/**
 * @brief  Floating point type flag for this library.
 */
#define CVX_FLOAT CVX_DEFAULT_FLOAT


// `Mat` default element type.
#if (CVX_DEFAULT_FLOAT == CV_32F)

#define CVX_FC1 CV_32FC1
#define CVX_FC2 CV_32FC2
#define CVX_FC3 CV_32FC3
#define CVX_FC4 CV_32FC4
#define CVX_FC(n) CV_32FC(n)

#elif (CVX_DEFAULT_FLOAT == CV_64F)

#define CVX_FC1 CV_64FC1
#define CVX_FC2 CV_64FC2
#define CVX_FC3 CV_64FC3
#define CVX_FC4 CV_64FC4
#define CVX_FC(n) CV_64FC(n)

#else

#error Bad default floating point size!

#endif // (CVX_DEFAULT_FLOAT == CV_32F)


/**
 * @brief       Extensions for OpenCV library.
 */
namespace cvx {

    // Import `namespace cv` into this namespace.

    using namespace ::cv;


    // Native floating point types.

    /**
     * @brief       Alias for ten-bytes (80-bit) floating-point
     *              number format.
     */
    typedef long double Real10;


    /**
     * @brief       Alias for eight-bytes (64-bit) floating-point
     *              number format.
     */
    typedef double Real8;


    /**
     * @brief       Alias for four-bytes (32-bit) floating-point
     *              number format.
     */
    typedef float Real4;


    /**
     * @brief      Alias for a temporary floating-point real format.
     *
     * @details    Ten-bytes (80-bit) floating-point number is the data type
     *             used internally in x86 based processors as temporary real
     *             format for floating-point instructions.
     */
    typedef Real10 RealT;


    /**
     * @brief      Alias for 64-bit floating-point real matrix.
     */
    typedef Mat_< Real8 > Matrix1d;


    /**
     * @brief      Alias for 32-bit floating-point real matrix.
     */
    typedef Mat_< Real4 > Matrix1f;


#if (CVX_DEFAULT_FLOAT == CV_32F)

    /**
     * @brief      Alias for the default floating-point real format.
     *
     * @details    Four-bytes (32-bit) floating-point number is the data type
     *             chosen as our default real number approximation data type.
     */
    typedef Real4 Real;

#elif (CVX_DEFAULT_FLOAT == CV_64F)

    /**
     * @brief      Alias for the default floating-point real format.
     *
     * @details    Eight-bytes (64-bit) floating-point number is the data type
     *             chosen as our default real number approximation data type.
     */
    typedef Real8 Real;

#endif // (CVX_DEFAULT_FLOAT == CV_32F)


#if (CVX_DEFAULT_FLOAT == CV_32F)

    /**
     * @brief      Alias for the default floating-point real 2D and 3D points.
     */
    typedef Point2f Point2r;
    typedef Point3f Point3r;

#elif (CVX_DEFAULT_FLOAT == CV_64F)

    /**
     * @brief      Alias for the default floating-point real 2D point.
     */
    typedef Point2d Point2r;
    typedef Point3d Point3r;

#endif // (CVX_DEFAULT_FLOAT == CV_32F)


    /**
     * @brief      Alias for a matrix of intengers.
     */
    typedef Mat_< int > Matrix1i, Matrix;


    /**
     * @brief      Alias for a matrix of reals (default floating-point).
     */
    typedef Mat_< Real > Matrix1r;


    /**
     * @brief      Alias for a matrix of 2D (integer) points.
     */
    typedef Mat_< Point2i > Matrix2i;


    /**
     * @brief       Alias for a vector of integers.
     */
    typedef std::vector< int > Vector1i;


    /**
     * @brief       Alias for a vector of reals.
     */
    typedef std::vector< Real > Vector1r;


    /**
     * @brief       Alias for a vector of points.
     */
    typedef std::vector< Point2i > Vector2i;


} // namespace cvx


#endif // CVX_UTIL_IMPORTS_HPP__INCLUDED
