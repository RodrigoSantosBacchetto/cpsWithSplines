/**
 * @file        CentroidDistanceFunction.hpp
 *
 * @brief       Defines a Centroid Distance Function shape signature computing
 *              functor.
 *
 * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 *
 * @copyright   Copyright © 2015, National University of Asunción, all rights
 *              reserved.
 *
 * This file is part of The Computer Vision Tools Project. It is subject to the
 * license terms in the [LICENSE.md](../license/LICENSE.md) file found in the
 * `licenses` sub-folder of the top-level directory of this distribution and at
 * https://bitbucket.org/cpslccauna/CpSignature/license/LICENSE.md. No part of
 * this project, including this file, may be copied, modified, propagated, or
 * distributed except according to the terms contained in the above LICENSE
 * file.
 *
 * See the [COPYING.md](../COPYING.md) file at the top-level directory of this
 * distribution and at https://bitbucket.org/cpslccauna/CpSignature/COPYING.md
 * for more legal information.
 */

#ifndef CbVX_DESCRIPTOR_CENTROID_DISTANCE_FUNCTION_HPP__INCLUDED
#define CbVX_DESCRIPTOR_CENTROID_DISTANCE_FUNCTION_HPP__INCLUDED

#pragma once

#include "Curve.hpp"
#include "Signature.hpp"


namespace cvx {

    class CentroidDistanceFunction {
    private:

        static const char cdf_signature[];

    public:

        typedef Signature_< cdf_signature > signature_t;

#if defined(__GNUC__)

        CentroidDistanceFunction() = default;

        CentroidDistanceFunction(const CentroidDistanceFunction&) = default;

        CentroidDistanceFunction(CentroidDistanceFunction&&) = default;

        ~CentroidDistanceFunction() = default;

        CentroidDistanceFunction& operator=(const CentroidDistanceFunction&) = default;

        CentroidDistanceFunction& operator=(CentroidDistanceFunction&&) = default;

#endif

        signature_t operator()(const Curve& contour,
                const unsigned int N = 128) const;

    };


    typedef CentroidDistanceFunction::signature_t CdfSignature;


} // namespace cvx


#endif // CbVX_DESCRIPTOR_CENTROID_DISTANCE_FUNCTION_HPP__INCLUDED
