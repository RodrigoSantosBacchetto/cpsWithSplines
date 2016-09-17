/**
 * @file        PlanarSet.cpp
 *
 * @brief       Implements a class of objects that represent planar point sets.
 *
 * @author      Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 *
 * @copyright   Copyright © 2015, National University of Asunción, all rights
 *              reserved.
 * @file        PlanarSet.
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

#include "Boundary.hpp"
#include "Compatibility.hpp"
#include "Curve.hpp"
#include "PlanarSet.hpp"
#include "Polygon.hpp"
#include <cassert>


namespace cvx {

    Boundary PlanarSet::boundary(const bool outer) const {
        Boundary::boundary_t boundary;
        Boundary::hierarchy_t hierarchy;
        if (outer) {
            findContours(data(), boundary, hierarchy,
                    CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        } else {
            findContours(data(), boundary, hierarchy,
                    CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
        }
        return Boundary(std::move(boundary), std::move(hierarchy));
    }

    void PlanarSet::set(const Curve& boundary, const bool fill) {
        const Size size = boundingRect(boundary.data()).br() + Point(2, 2);
        _frame = frame_t::zeros(size);
        Boundary::boundary_t contours;
        contours.push_back(boundary.data());
        if (fill) {
            drawContours(_frame, contours, 0, 255, FILLED, 8);
        } else {
            drawContours(_frame, contours, 0, 255, 1, 8);
        }
    }

    void PlanarSet::set(const Polygon& boundary, const bool fill) {
        const Size size = boundingRect(boundary.data()).br() + Point(2, 2);
        _frame = frame_t::zeros(size);
        Boundary::boundary_t contours;
        contours.push_back(boundary.data());
        if (fill) {
            drawContours(_frame, contours, 0, 255, FILLED, 8);
        } else {
            drawContours(_frame, contours, 0, 255, 1, 8);
        }
    }

    void PlanarSet::set(const Boundary& silhouette, const bool fill) {
        Point br(0, 0);
        for (const auto& cv : silhouette.slice(0)) {
            const Point cr = boundingRect(cv).br();
            if (cr.x > br.x) {
                br.x = cr.x;
            } else if (cr.y > br.y) {
                br.y = cr.y;
            }
        }
        const Size size = br + Point(2, 2);
        _frame = frame_t::zeros(size);
        if (fill) {
            drawContours(_frame, silhouette.data(), -1, 255,
                FILLED, 8, silhouette.hierarchy(), 2);
        } else {
            drawContours(_frame, silhouette.data(), -1, 255,
                1, 8, silhouette.hierarchy(), 2);
        }
    }

    void PlanarSet::binarize(const int algorithm) {
        // Binarize using Otsu's algorithm.
        const frame_t& src = _frame;
        frame_t dst;
        threshold(src, dst, 0, 255, CV_THRESH_BINARY | algorithm);
        _frame = std::move(dst);
    }

    /**
     * @brief       Implements OpenCV XML/YAML deserialization.
     * @param[in]   fs Reference to a `FileStorage` object.
     */
    void PlanarSet::read(const FileNode& fn, const PlanarSet&) {
        // Deserialize the shape data.
        frame_t shape;
        fn["map"] >> shape;
        CV_Assert(shape.rows > 0 and shape.cols > 0);
        set(std::move(shape));
    }

    /**
     * @brief       Implements OpenCV XML/YAML serialization.
     * @param[in]   fs Reference to a `FileStorage` object.
     * @param[in]   name Reference to root element name.
     */
    void PlanarSet::write(FileStorage& fs, const String&) const {
        // Serialize shape data.
        CV_Assert(_frame.rows > 0 and _frame.cols > 0);
        fs << "{" << "map" << _frame << "}";
    }

} // namespace cvx
