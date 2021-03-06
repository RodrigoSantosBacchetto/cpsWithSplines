/**
 * @file        NormL1.hpp
 *
 * @brief       Defines a L1 norm functor template class.
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

#ifndef CVX_METRIC_NORM_L1_HPP__INCLUDED
#define CVX_METRIC_NORM_L1_HPP__INCLUDED

#pragma once

#include "NormLp.hpp"


namespace cvx {

    /**
     * @class       NormLp_< 1 >
     *
     * @brief       Template class for L1 (also kown as cityblock, taxicab, or
     *              Manhattan) norm functor objects.
     *
     * @details     Objects instantiated from this class computes the distance
     *              between a point and the origin, or the length of a vector,
     *              using the L1 norm.
     *
     * @author      Luis Nasi < luiginasi12@gmail.com >
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     *
     * @date        2015-03-19
     */
    template<>
    class NormLp_< 1 > {

    public:

        // Use default cannon for constructors, destructor, and assignment.

        /**
         * @brief       Compute the norm of a 2D vector.
         *
         * @param[in]   v  A constant reference to the 2D vector.
         *
         * @return      The norm of vector `v`, a floating point value
         *              of type `Real`.
         */
        template< typename Type >
        Real operator()(const Point_< Type >& v) const {
            return norm1_2(v);
        }


    private:

        template< typename PointType > static
        Real norm1_2(const PointType& p) {
            return static_cast< Real > (std::abs(p.x) + std::abs(p.y));
        }

    };


    /**
     * @brief      An alias for `NormLp_< 1 >`.
     */
    typedef NormLp_< 1 > NormL1;


} // namespace cvx


#endif // CVX_METRIC_NORM_L1_HPP__INCLUDED
