/**
 * @file        EqualLengthSampling.hpp
 *
 * @brief       Defines a class to perform polyline simplification using equal
 *              arc-legth spacing sampling algorithm.
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

#ifndef CVX_ALGORITHM_EQUAL_LENGTH_SAMPLING_HPP__INCLUDED
#define	CVX_ALGORITHM_EQUAL_LENGTH_SAMPLING_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Arc.hpp"
#include "Curve.hpp"


namespace cvx {


    class EqualLengthSampling {

    public:

        EqualLengthSampling() :
        _offset(0) {
            // NOOP
        }


        EqualLengthSampling(const EqualLengthSampling& src) :
        _offset(src._offset) {
            // NOOP
        }


        EqualLengthSampling(EqualLengthSampling&& src) :
        _offset(std::move(src._offset)) {
            // NOOP
        }


        explicit
        EqualLengthSampling(const Real offset) :
        _offset(offset) {
            // NOOP
        }


        ~EqualLengthSampling() {
            // NOOP
        }


        EqualLengthSampling& operator=(const EqualLengthSampling& src) {
            return assign(src);
        }


        EqualLengthSampling& operator=(EqualLengthSampling&& src) {
            return assign(std::move(src));
        }


        EqualLengthSampling& assign(const EqualLengthSampling& src) {
            return (_offset = src._offset, *this);
        }


        EqualLengthSampling& assign(EqualLengthSampling&& src) {
            return (_offset = std::move(src._offset), *this);
        }


        void offset(const Real offset) {
            _offset = offset;
        }


        const Real offset() const {
            return _offset;
        }


        /**
         * @brief       Resample the current open path.
         *
         * @details     The method should be used commonly for subsampling
         *              tasks. However, if needed, e.g., for adapting small
         *              or very short paths, the method will return the
         *              required points, some of them will be repeated anyway.
         *
         * @param[in]   size  The amount of points in the resampled path.
         *
         * @return      A vector with the indices of the points of the
         *              in the resampled path, representing a(n) (open)
         *              crooked line.
         */
        Vector1i simplify_open(const Arc& polyline,
                               const unsigned size = 128) const {
            return simplify_open_(polyline.length.data(), polyline.length(),
                                  size);
        }


        /**
         * @brief       Resample the current path.
         *
         * @details     The method should be used commonly for subsampling
         *              tasks. However, if needed, e.g., for adapting small
         *              or very short paths, the method will return the
         *              required points, some of them will be repeated anyway.
         *
         * @param[in]   size  The amount of points in the resampled contour.
         *
         * @return      A vector with the `n` points of the resampled path,
         *              representing a (closed) polygon.
         */
        Vector1i simplify_closed(const Curve& polyline,
                                 const unsigned size = 128) const {
            return simplify_closed_(polyline.length.data(), polyline.length(),
                                    size, _offset);
        }


        /**
         * @brief       Resample the current path.
         *
         * @details     The method should be used commonly for subsampling
         *              tasks. However, if needed, e.g., for adapting small
         *              or very short paths, the method will return the
         *              required points, some of them will be repeated anyway.
         *
         * @param[in]   size  The amount of points in the resampled contour.
         *
         * @param[in]   offset  An offset of the first path's point to extract
         *              with respect to the path's starting point. This value
         *              must be a fraction, in [0, 1), of the sampling norm,
         *              length / n.
         *
         * @return      A vector with the `n` points of the resampled path,
         *              representing a (closed) polygon.
         */
        Vector1i simplify_closed(const Curve& polyline,
                                 const unsigned size,
                                 const Real offset) const {
            return simplify_closed_(polyline.length.data(), polyline.length(),
                                    size, offset);
        }


        /**
         * Check if this is an uniform sampling algorithm.
         */
        static
        bool uniform() {
            return true;
        }


    private:

        static
        Vector1i simplify_open_(const Vector1r& polylength,
                                const Real length,
                                const unsigned size) {
            Real s = 0;
            const Real norm = length / (Real) (size - 1);
            Vector1i samples;
            for (unsigned i = 0, j = 1, k = 0; k < size; ++k, s += norm) {
                // Find the lower bound.
                while (j < polylength.size()) {
                    if (s <= polylength[j]) {
                        if (s - polylength[i] < polylength[j] - s) {
                            samples.push_back((int) i);
                        } else {
                            samples.push_back((int) j);
                        }
                        break;
                    } else {
                        i = j++;
                    }
                }
                if (j == polylength.size()) {
                    samples.push_back(i);
                }
            }
            CV_Assert(samples.size() == size);
            return samples;
        }


        static
        Vector1i simplify_closed_(const Vector1r& polylength,
                                  const Real length,
                                  const unsigned size,
                                  const Real offset) {
            Vector1i samples;
            const Real norm = length / (Real) size;
            Real s = offset * norm;
            for (unsigned i = 0, j = 1, k = 0; k < size; ++k, s += norm) {
                // Find the lower bound.
                while (j < polylength.size()) {
                    if (s <= polylength[j]) {
                        if (s - polylength[i] < polylength[j] - s) {
                            samples.push_back((int) i);
                        } else {
                            samples.push_back((int) j);
                        }
                        break;
                    } else {
                        i = j++;
                    }
                }
                if (j == polylength.size()) {
                    if (s - polylength[i] < length - s) {
                        samples.push_back(i);
                    } else {
                        samples.push_back(0);
                    }
                }
            }
            CV_Assert(samples.size() == size);
            return samples;
        }


    private:

        Real _offset;

    };


} // namespace cvx


#endif // CVX_ALGORITHM_EQUAL_LENGTH_SAMPLING_HPP__INCLUDED
