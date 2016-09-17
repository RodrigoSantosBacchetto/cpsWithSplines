/**
 * @file        Metric.hpp
 *
 * @brief       Norm and distance functor classes.
 *
 * @details     Defines norm and distance functors for points, vectors,
 *              generic matrices or arrays, descriptors and signatures.
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

#ifndef CVX_METRIC_HPP__INCLUDED
#define CVX_METRIC_HPP__INCLUDED

#pragma once


// Norm and distance functor metaclasses.

#include "NormLp.hpp"
#include "Distance.hpp"
#include "DistanceConnective.hpp"


// Norm functors classes.

#include "NormL1.hpp"
#include "NormL2.hpp"
#include "NormL2Log.hpp"
#include "NormL2Squared.hpp"
#include "NormLinf.hpp"


// Geometric dot connector classes.

#include "ConnectorL2.hpp"


// Distance functors classes.

#include "DistanceL1.hpp"
#include "DistanceL2.hpp"
#include "DistanceL2Log.hpp"
#include "DistanceL2Squared.hpp"
#include "DistanceLinf.hpp"
#include "InnerDistanceL2.hpp"
#include "InnerDistanceL2Log.hpp"
#include "InnerDistanceL2Squared.hpp"


#endif // CVX_METRIC_HPP__INCLUDED
