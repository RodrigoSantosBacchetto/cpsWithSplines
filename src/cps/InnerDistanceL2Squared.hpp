/**
 * @file        InnerDistanceL2Squared.hpp
 *
 * @brief       Defines a L2 squared-distance functor template class.
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

#ifndef CVX_METRIC_INNER_DISTANCE_L2_SQUARED_HPP__INCLUDED
#define CVX_METRIC_INNER_DISTANCE_L2_SQUARED_HPP__INCLUDED

#pragma once

#include "ConnectorL2.hpp"
#include "DistanceConnective.hpp"
#include "NormL2Squared.hpp"


namespace cvx {

    /**
     * @brief      The inner-euclidean squared-distance functor.
     */
    typedef DistanceConnective_< NormL2Squared, ConnectorL2 >
                InnerDistanceL2Squared,
                InnerDistanceEuclideanSquared;

}


#endif // CVX_METRIC_INNER_DISTANCE_L2_SQUARED_HPP__INCLUDED
