/**
 * @file        CentroidDistanceFunction.cpp
 *
 * @brief       Implements a Centroid Distance Function shape signature functor.
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

#include "CentroidDistanceFunction.hpp"
#include "DistanceL2.hpp"
#include "LineSimplifier.hpp"
#include "EqualLengthSampling.hpp"


namespace cvx {

    const char CentroidDistanceFunction::cdf_signature[] = "cdf-signature";

    CdfSignature CentroidDistanceFunction::operator()(const Curve& contour,
            const unsigned int N) const {
        // Find the centroid.
        const Moments moment = moments(contour.data());
        const Point2r c = Point2r(moment.m10, moment.m01) / moment.m00;
        // Use the square root of the area as the norm.
        const Real inorm = 1 / std::sqrt(moment.m00);
        // Resample the contour.
        static LineSimplifier_< EqualLengthSampling > sampler;
        const Polygon nodes = sampler(contour, N);
        // Compute the discrete centroid distance function.
        static DistanceEuclidean dist;
        std::vector< Real > sig(N);
        for (const Point& i : nodes.data()) {
            Point2r p = i;
            sig.push_back(dist(p, c) * inorm);
        }
        return std::move(sig);
    }

} // namespace cvx
