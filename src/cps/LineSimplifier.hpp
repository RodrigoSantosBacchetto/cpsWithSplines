/**
 * @file        LineSimplifier.hpp
 *
 * @brief       Polyline simplification using line approximation algorithms.
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

#ifndef CVX_ALGORITHM_LINE_SIMPLIFIER_HPP__INCLUDED
#define	CVX_ALGORITHM_LINE_SIMPLIFIER_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Arc.hpp"
#include "PolyLine.hpp"
#include "Curve.hpp"
#include "Polygon.hpp"


namespace cvx {


    template < typename ApproximationAlgorithm >
    class LineSimplifier_ {

    public:

        typedef  ApproximationAlgorithm algorithm_t;


    public:

        LineSimplifier_() {
            // NOOP
        }


        LineSimplifier_(const LineSimplifier_& src) :
        algorithm(src.algorithm) {
            // NOOP
        }


        LineSimplifier_(LineSimplifier_&& src) :
        algorithm(std::move(src.algorithm)) {
            // NOOP
        }


        explicit
        LineSimplifier_(const algorithm_t& src) :
        algorithm(src) {
            // NOOP
        }


        explicit
        LineSimplifier_(algorithm_t&& src) :
        algorithm(std::move(src)) {
            // NOOP
        }


        ~LineSimplifier_() {
            // NOOP
        }


        LineSimplifier_& operator=(const LineSimplifier_& src) {
            return assign(src);
        }


        LineSimplifier_& operator=(LineSimplifier_&& src) {
            return assign(std::move(src));
        }


        LineSimplifier_& assign(const LineSimplifier_& src) {
            return (algorithm = src.algorithm, *this);
        }


        LineSimplifier_& assign(LineSimplifier_&& src) {
            return (algorithm = std::move(src.algorithm), *this);
        }

 
        PolyLine operator()(const Arc& polyline) const {
            Vector1i d = algorithm.simplify_open(polyline);
#if defined(_OPENMP)
            Vector2i c(d.size());
            //#pragma omp parallel for
            for (int i = 0; (unsigned)i < d.size(); ++i) {
                c[i] = polyline[d[i]];
            }
#else
            Vector2i c;
            for (const int i : d) {
                c.push_back(polyline[i]);
            }
#endif // defined(_OPENMP)
            return PolyLine(c);
        }
 

        PolyLine operator()(const Arc& polyline,
                            const unsigned size) const {
            Vector1i d = algorithm.simplify_open(polyline, size);
#if defined(_OPENMP)
            Vector2i c(size);
            //#pragma omp parallel for
            for (int i = 0; (unsigned)i < size; ++i) {
                c[i] = polyline[d[i]];
            }
#else
            Vector2i c;
            for (const int i : d) {
                c.push_back(polyline[i]);
            }
#endif // defined(_OPENMP)
            return PolyLine(c);
        }


        Polygon operator()(const Curve& polyline) const {
            Vector1i d = algorithm.simplify_closed(polyline);
#if defined(_OPENMP)
            Vector2i c(d.size());
            //#pragma omp parallel for
            for (int i = 0; (unsigned)i < d.size(); ++i) {
                c[i] = polyline[d[i]];
            }
#else
            Vector2i c;
            for (const int i : d) {
                c.push_back(polyline[i]);
            }
#endif // defined(_OPENMP)
            return Polygon(c);
        }


        Polygon operator()(const Curve& polyline,
                           const unsigned size) const {
            Vector1i d = algorithm.simplify_closed(polyline, size);
#if defined(_OPENMP)
            Vector2i c(size);
            //#pragma omp parallel for
            for (int i = 0; (unsigned)i < size; ++i) {
                c[i] = polyline[d[i]];
            }
#else
            Vector2i c;
            for (const int i : d) {
                c.push_back(polyline[i]);
            }
#endif // defined(_OPENMP)
            return Polygon(c);
        }


    public:

        algorithm_t algorithm;

    };


} // namespace cvx


#endif // CVX_ALGORITHM_LINE_SIMPLIFIER_HPP__INCLUDED
