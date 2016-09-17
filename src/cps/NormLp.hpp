/**
 * @file        NormLp.hpp
 *
 * @brief       Defines a Lp norm functor class template (metaclass).
 *
 * @remark      This class template is the prototype, for all norm functors
 *              metaclasses and classes, i.e., it defines the interface for
 *              norm functors.
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

#ifndef CVX_METRIC_NORM_LP_HPP__INCLUDED
#define CVX_METRIC_NORM_LP_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Math.hpp"


namespace cvx {

    /**
     * @tclass      NormLp_
     *
     * @brief       Metaclass for Lp (also kown as Minkowski or minkowskian)
     *              norm functor classes.
     *
     * @details     Objects of classes instantiated from this class template
     *              computes the distance between a point and the origin, or
     *              the length of a vector, using the Lp norm.
     *
     * @tparam      P  The degree of the Lp norm.
     *
     * @remark      This class template is the prototype, for all norm functor
     *              metaclasses and classes, i.e., it defines the interface for
     *              norm functors. Each implementation of a norm functor class
     *              **must** have, at least, a default constructor, destructor,
     *              and the function call `operator()` for 2D vectors.
     *
     * @remark      Special cases requiring additional information can have
     *              additional constructors, methods and data members as
     *              needed. The default constructor must initialize a default
     *              set of data members. If a case can't be applied without
     *              explicit initialization, the default constructor must
     *              throw an exception indicating the misuse of the functor.
     *
     * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     *
     * @version     1.0
     *
     * @date        2015-03-19
     */
    template< unsigned int P >
    class NormLp_ {

    public:

        // Use default cannon for constructors, destructor, and assignment.

        /**
         * @brief       Compute the norm of a 2D vector.
         *
         * @details     This member effectively computes the norm of a 2D
         *              vector, by itself or by calling auxiliar methods.
         *
         * @remark      This is a mandatory member function.
         *
         * @param[in]   v  A constant reference to the 2D vector.
         *
         * @return      The norm of vector `v`, a floating point value
         *              of type `Real`.
         */
        template< typename Type >
        Real operator()(const Point_< Type >& v) const {
            return normP_2<Type, P>(v);
        }


    private:

        template< typename Type > static
        Real normP_2(const Point_< Type >& p) {
            return norm_lp<Type, P>(v);
        }

    };


} // namespace cvx


#endif // CVX_METRIC_NORM_LP_HPP__INCLUDED
