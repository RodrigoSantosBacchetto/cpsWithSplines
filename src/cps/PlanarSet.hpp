/**
 * @file        PlanarSet.hpp
 *
 * @brief       Defines a class of objects that represent dense planar point
 *              sets.
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

#ifndef CVX_GEOMETRY_PLANAR_SET_HPP__INCLUDED
#define CVX_GEOMETRY_PLANAR_SET_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Persistence.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>


// Forward declaration for class serialization mechanism.
CVX_DECLARE_CLASS_PERSISTENCE_PROXIES(PlanarSet)


namespace cvx {


    // Forward declarations for Boundary and Curve classes.

    class Boundary;
    class ConnectorL2;
    class Curve;
    class Polygon;


    /**
     * @brief   A class of objects that represent a discrete approximation of
     *          dense planar (2D) point sets.
     *
     * @details Objects of this class store a rectangular region as a binary
     *          pixel map. Whithin that region, each non-zero valued pixel is
     *          assumed to be part of the interest point set, and each zero
     *          valued pixel is a point lying outside the set.
     */
    class PlanarSet {

    public:

        /**
         * Type for pixel data.
         */
        typedef unsigned char value_t;


        /**
         * Type for set's bounding frame.
         */
        typedef Mat_< value_t > frame_t;


        /**
         * Type for image writing parameter vector.
         */
        typedef Vector1i params_t;


    public:

        /**
         * @brief       Constructs a null set.
         */
        PlanarSet() {
            // NOOP
        }


        /**
         * @brief       Constructs a copy of a given `PlanarSet` object.
         *
         * @param[in]   src  A constant reference to a `PlanarSet` object.
         */
        PlanarSet(const PlanarSet& src) :
        _frame(src._frame) {
            // NOOP
        }


        /**
         * @brief       Constructs a `PlanarSet` by transferring data from
         *              a source `PlanarSet` object.
         *
         * @param[in]   src  A rvalue reference to a `PlanarSet` object.
         */
        PlanarSet(PlanarSet&& src) :
        _frame(std::move(src._frame)) {
            // NOOP
        }


        /**
         * @brief       Constructs a `PlanarSet` from a given `Curve` object.
         *
         * @param[in]   boundary  A constant reference to a `Curve` object
         *              representing the boundary of the point set.
         *
         * @param[in]   fill  If true, the method fills the boundary with the
         *              set interior points. If false (default) it only draws
         *              the boundary points.
         */
        PlanarSet(const Curve& boundary, const bool fill = false) {
            set(boundary, fill);
        }


        /**
         * @brief       Constructs a `PlanarSet` from a given `Curve` object.
         *
         * @param[in]   boundary  A constant reference to a `Polygon` object
         *              representing the boundary of the point set.
         *
         * @param[in]   fill  If true, the method fills the boundary with the
         *              set interior points. If false (default) it only draws
         *              the boundary points.
         */
        PlanarSet(const Polygon& boundary, const bool fill = false) {
            set(boundary, fill);
        }


        /**
         * @brief       Constructs a `PlanarSet` from a given `Boundary`
         *              object.
         *
         * @param[in]   boundary  A constant reference to a `Boundary` object
         *              holding the collection of curves representing the
         *              boundary of the point set.
         *
         * @param[in]   fill  If true, the method fills the boundary with the
         *              set interior points. If false (default) it only draws
         *              the boundary points.
         */
        PlanarSet(const Boundary& boundary, const bool fill = false) {
            set(boundary, fill);
        }


        /**
         * @brief       Constructs a `PlanarSet` by copying data from a
         *              provided `Mat` object holding the region of interest
         *              of a point set.
         *
         * @param[in]   frame  A constant reference to a `Mat` object.
         *
         * @param[in]   binarize  If true the method binarizes the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @param[in]   invert  If true the method inverts the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @remark      Binarization is done using Otsu's algorithm.
         */
        explicit
        PlanarSet(const frame_t& frame) :
        _frame(frame) {
            // NOOP
        }


        /**
         * @brief       Constructs a `PlanarSet` by transferring data from a
         *              provided `Mat` object holding the region of interest
         *              of a point set.
         *
         * @param[in]   frame  A rvalue reference to a `Mat` object.
         *
         * @param[in]   binarize  If true the method binarizes the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @param[in]   invert  If true the method inverts the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @remark      Binarization is done using Otsu's algorithm.
         */
        explicit
        PlanarSet(frame_t&& frame) :
        _frame(std::move(frame)) {
            // NOOP
        }


        /**
         * @brief       Destructor.
         */
        ~PlanarSet() {
            // NOOP
        }


        /**
         * @brief       Copies of a given `PlanarSet` object.
         *
         * @param[in]   src  A constant reference to a `PlanarSet` object.
         *
         * @return      A reference to this object.
         */
        PlanarSet& operator=(const PlanarSet& src) {
            return assign(src);
        }


        /**
         * @brief       Transfers data from a source `PlanarSet` object.
         *
         * @param[in]   src  A rvalue reference to a `PlanarSet` object.
         *
         * @return      A reference to this object.
         */
        PlanarSet& operator=(PlanarSet&& src) {
            return assign(std::move(src));
        }


        /**
         * @brief       Copy assignment. Copies of a given `PlanarSet` object.
         *
         * @param[in]   src  A constant reference to a `PlanarSet` object.
         *
         * @return      A reference to this object.
         */
        PlanarSet& assign(const PlanarSet& src) {
            return (_frame = src._frame, *this);
        }


        /**
         * @brief       Move assignment. Transfers data from a source
         *              `PlanarSet` object.
         *
         * @param[in]   src  A rvalue reference to a `PlanarSet` object.
         *
         * @return      A reference to this object.
         */
        PlanarSet& assign(PlanarSet&& src) {
            return (_frame = std::move(src._frame), *this);
        }


        /**
         * @brief       (Re)initializes this `PlanarSet` object by setting the
         *              boundary of a point set from a given `Boundary` object.
         *
         * @param[in]   boundary  A constant reference to a `Boundary` object
         *              holding the collection of curves representing the
         *              boundary of the point set.
         *
         * @param[in]   fill  If true, the method fills the boundary with the
         *              set interior points. If false (default) it only draws
         *              the boundary points.
         */
        void set(const Boundary& boundary, const bool fill = false);


        /**
         * @brief       (Re)initializes this `PlanarSet` object by setting the
         *              boundary of a point set from a given `Curve` object.
         *
         * @param[in]   boundary  A constant reference to a `Curve` object
         *              representing the boundary of the point set.
         *
         * @param[in]   fill  If true, the method fills the boundary with the
         *              set interior points. If false (default) it only draws
         *              the boundary points.
         */
        void set(const Curve& boundary, const bool fill = false);


        /**
         * @brief       (Re)initializes this `PlanarSet` object by setting the
         *              boundary of a point set from a given `Curve` object.
         *
         * @param[in]   boundary  A constant reference to a `Polygon` object
         *              representing the boundary of the point set.
         *
         * @param[in]   fill  If true, the method fills the boundary with the
         *              set interior points. If false (default) it only draws
         *              the boundary points.
         */
        void set(const Polygon& boundary, const bool fill = false);


        /**
         * @brief       (Re)initializes this `PlanarSet` object by copying data
         *              from a provided `Mat` object holding the region of
         *              interest of a point set.
         *
         * @param[in]   frame  A constant reference to a `Mat` object.
         *
         * @param[in]   binarize  If true the method binarizes the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @param[in]   invert  If true the method inverts the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @remark      Binarization is done using Otsu's algorithm.
         */
        void set(const frame_t& frame) {
            _frame = frame;
        }


        /**
         * @brief       (Re)initializes this `PlanarSet` object by transferring
         *              data from a provided `Mat` object holding the region of
         *              interest of a point set.
         *
         * @param[in]   frame  A rvalue reference to a `Mat` object.
         *
         * @param[in]   binarize  If true the method binarizes the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @param[in]   invert  If true the method inverts the region after
         *              being assigned. If false (default) the region is leaved
         *              as is.
         *
         * @remark      Binarization is done using Otsu's algorithm.
         */
        void set(frame_t&& frame) {
            _frame = std::move(frame);
        }


        /**
         * @brief       Get the value of the ij-th pixel in the rasterized
         *              region.

         * @details     The method retrieves the value of the ij-th pixel
         *              in the wrapped rasterized region. If the returned
         *              value is non-zero, the point belongs to the point
         *              set.
         *
         * @param[in]   i  Index to the i-th row in the data matrix.
         *
         * @param[in]   j  Index to the j-th column in the data matrix.
         *
         * @return      The ij-th pixels' value as an unsigned value of
         *              type `value_t`.
         */
        value_t operator()(const unsigned i, const unsigned j) const {
            return _frame(i, j);
        }


        /**
         * @brief       Get the value of the ij-th pixel in the rasterized
         *              region.
         *
         * @details     The method retrieves the value of the ij-th pixel
         *              in the wrapped rasterized region. If the returned
         *              value is non-zero, the point belongs to the point
         *              set.
         *
         * @param[in]   i  Index to the i-th row in the data matrix.
         *
         * @param[in]   j  Index to the j-th column in the data matrix.
         *
         * @return      The ij-th pixels' value as an unsigned value of
         *              type `value_t`.
         */
        value_t at(const unsigned i, const unsigned j) const {
            return _frame.at< value_t >(i, j);
        }


        /**
         * @brief       Binarize the current image map.
         *
         * @param[in]   algorithm  Algoritm flag.
         *
         * @remark      By deafult, binarization is done using Otsu's
         *              algorithm.
         */
        void binarize(const int algorithm = CV_THRESH_OTSU);


        /**
         * @brief       Get the boundary of the point set contained in the
         *              region of interest wrapped by this `PlanarSet` object.
         *
         * @param[in]   outer  A boolean value, if true, the query only returns
         *              the outer boundary of the point set.
         *
         * @return      A `Boundary` object holding the collection of curves
         *              representing the boundary of the point set.
         */
        Boundary boundary(const bool outer = false) const;


        /**
         * @brief       Accessor to the rasterized region data.
         *
         * @return      A constant reference to a `Mat` object holding the
         *              region of interest data for the poit set.
         */
        const frame_t& data() const {
            return _frame;
        }


        /**
         * @brief       Loads an image from a file. Reads an image file as
         *              the rasterized region defining the point set, and
         *              (re)initializes this `PlanarSet` object.
         *
         * @param[in]   fileName  The path and name of a source image file.
         *
         * @param[in]   flags  Flags specifying the color type of a loaded
         *              image (see `cv::ImreadModes`):
         *              - `IMREAD_UNCHANGED` 8-bit, color or not (default).
         *                  Load the image as is (with alpha
         *                  channel, otherwise it gets cropped).
         *              - `IMREAD_GRAYSCALE` 8bit, gray. Always convert
         *                  image to the single channel grayscale image.
         *              - `IMREAD_COLOR` ?, color. Always convert image to
         *                  the 3 channel BGR color image.
         *              - `IMREAD_ANYDEPTH` any depth, ? Read 16-bit/32-bit
         *                  image when the input has the corresponding depth,
         *                  otherwise convert it to 8-bit.
         *              - `IMREAD_ANYCOLOR` ?, any color. The image is read
         *                  in any possible color format.
         *              - `IMREAD_LOAD_GDAL` If set, use the gdal driver for
         *                  loading the image.
         *
         * @remark      The function determines the type of an image by the
         *              content, not by the file extension.
         */
        void imread(const std::string& fileName,
                    const int flags = IMREAD_UNCHANGED) {
            _frame = ::cv::imread(fileName, flags);
        }


        /**
         * @brief       Saves an image to a specified file. Writes an image
         *              file with the rasterized region defining the point
         *              set stored in this `PlanarSet` object.
         *
         * @param[in]   fileName  The path and name of the destination
         *              image file.
         *
         * @param[in]   params  Format-specific save parameters encoded as
         *              pairs `paramId_1, paramValue_1, paramId_2,
         *              paramValue_2, ...` The following parameters are
         *              currently supported (see `cv::ImwriteFlags` and
         *              `cv::ImwritePNGFlags`).
         *
         * @remark      The image format is chosen based on the filename
         *              extension (see `cv::imread()` for the list of
         *              extensions).
         */
        bool imwrite(const std::string& fileName,
                     const params_t& params = params_t()) const {
            return ::cv::imwrite(fileName, _frame, params);
        }


        /**
         * @brief       Inverts the current image map.
         */
        void invert() {
            ::cv::subtract(Scalar::all(255), _frame, _frame);
        }


    private:

        frame_t _frame; /**< The (discrete) set point region data. */

        friend class ConnectorL2;


        CVX_DECLARE_CLASS_PERSISTENCE(PlanarSet, "planar-set")

    };


} // namespace cvx


// Definitions for class serialization mechanism.
CVX_DEFINE_CLASS_PERSISTENCE_PROXIES(PlanarSet)


#endif // CVX_GEOMETRY_PLANAR_SET_HPP__INCLUDED
