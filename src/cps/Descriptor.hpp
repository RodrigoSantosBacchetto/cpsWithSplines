/**
 * @file        Descriptor.hpp
 *
 * @brief       Defines a (discrete) shape descriptor metaclass.
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

#ifndef CVX_DESCRIPTOR_DESCRIPTOR_HPP__INCLUDED
#define CVX_DESCRIPTOR_DESCRIPTOR_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Persistence.hpp"


// Forward declaration for class serialization.
namespace cvx {

    template< typename FeatureType, const char* const NODENAME >
    class Descriptor_;

}


// Prototype declaration of proxy methods for class serialization.
namespace cv {

    template< typename FeatureType, const char* const NODENAME >
    void write(FileStorage&, const String&,
            const cvx::Descriptor_< FeatureType, NODENAME >&);

    template< typename FeatureType, const char* const NODENAME >
    void read(const FileNode&, cvx::Descriptor_< FeatureType, NODENAME >&,
            const cvx::Descriptor_< FeatureType, NODENAME >&);
}


namespace cvx {


    /** 
     * @tclass     Descriptor_
     *
     * @brief      Metaclass for shape descriptor classes.
     * @details    Objects of classes instantiated from this class template holds the
     *             data for descriptor descriptors.
     *
     * @tparam     FeatureType Represents a feature type class.
     *
     * @author     Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
     * @version    1.0
     * @date       2015-03-19
     */
    template< typename FeatureType, const char* const NODENAME >
    class Descriptor_ {

    public:

        typedef FeatureType feature_t; /**< The type of the features.*/
        typedef std::vector< FeatureType > vector_t; /**< The type of the feature vector. */


        /**
         * @brief      Default constructor.
         */
        Descriptor_() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         * @param[in]   src A constant reference to a source object.
         */
        Descriptor_(const Descriptor_& src) :
        _data(src._data) {
            // NOOP
        }


        /**
         * @brief           Move constructor.
         * @param[in,out]   tmp A rvalue reference to a source object.
         */
        Descriptor_(Descriptor_&& tmp) :
        _data(std::move(tmp._data)) {
            // NOOP
        }


        /**
         * @brief       Clone constructor.
         * @param[in]   src A constant reference to a compatible source object.
         */
        template< typename OtherFeatureType >
        Descriptor_(const Descriptor_< OtherFeatureType, NODENAME >& src) {
            for (const OtherFeatureType& i : src.data()) {
                this->data().push_back(static_cast< FeatureType > (i));
            }
        }


        /**
         * @brief       Copy wrapper constructor.
         * @param[in]   data A constant reference to a data vector.
         */
        Descriptor_(const vector_t& data) :
        _data(data) {
            // NOOP
        }


        /**
         * @brief           Move wrapper constructor.
         * @param[in,out]   data A rvalue reference to a data vector.
         */
        Descriptor_(vector_t&& data) :
        _data(std::move(data)) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~Descriptor_() {
            // NOOP
        }


        /**
         * @brief       Copy assignment operator.
         * @param[in]   src A constant reference to a source object.
         */
        Descriptor_& operator=(const Descriptor_& src) {
            return assign(src);
        }


        /**
         * @brief           Move assignment operator.
         * @param[in,out]   tmp A rvalue reference to a source object.
         */
        Descriptor_& operator=(Descriptor_&& tmp) {
            return assign(std::move(tmp));
        }


        /**
         * @brief       Copy assignment.
         * @param[in]   src A constant reference to a source object.
         */
        Descriptor_& assign(const Descriptor_& src) {
            return (_data = src._data, *this);
        }


        /**
         * @brief           Move assignment.
         * @param[in,out]   tmp A rvalue reference to a source object.
         */
        Descriptor_& assign(Descriptor_&& tmp) {
            return (_data = std::move(tmp._data), *this);
        }


        /**
         * @brief       Indexed access to the samples.
         * @param[in]   i The index of the required sample.
         * @return      The i-th feature.
         */
        feature_t operator[](unsigned i) const {
            return _data[i];
        }


        /**
         * @brief   Return the amount of samples in the contour.
         * @return  An unsigned integer with the amount of samples in the contour.
         */
        size_t size() const {
            return _data.size();
        }


        /**
         * @brief   Descriptor_ data (feature vector) accessor.
         * @return  A reference to the constant descriptor data.
         */
        const vector_t& data() const {
            return _data;
        }


        /**
         * @brief   Descriptor_ data (feature vector) accessor.
         * @return  A reference to the descriptor data.
         */
        vector_t& data() {
            return _data;
        }


        /**
         * @brief   Descriptor_ values (feature array) accessor.
         * @return  An array with the descriptor data.
         */
        Mat values() const {
            return Mat_< feature_t >(_data);
        }


        // In-class declaration for serialization.

        void writeTo(FileStorage& fs) const {
            fs << NODENAME << *this;
        }


        void writeTo(FileStorage&& fs) const {
            writeTo(fs);
        }


        void save(const std::string& fileName) const {
            writeTo(FileStorage(fileName, FileStorage::WRITE));
        }


        void readFrom(FileStorage& fs) {
            fs[NODENAME] >> *this;
        }


        void readFrom(FileStorage&& fs) {
            readFrom(fs);
        }


        void load(const std::string& fileName) {
            readFrom(FileStorage(fileName, FileStorage::READ));
        }


    private:

        void write(FileStorage& fs, const String& name) const;


        void read(const FileNode& fn, const Descriptor_& defVal);


        friend void write(FileStorage&, const String&,
                const Descriptor_&);


        friend void read(const FileNode&, Descriptor_&,
                const Descriptor_&);


        vector_t _data; /**< The (discrete) descriptor vector. */

    };


} // namespace cvx


// Definition of proxy methods for class serialization.
namespace cv {

    template< typename FeatureType, const char* const NODENAME >
    void write(FileStorage& fs, const String& name,
            const cvx::Descriptor_< FeatureType, NODENAME >& x) {
        x.write(fs, name);
    }

    template< typename FeatureType, const char* const NODENAME >
    void read(const FileNode& fn, cvx::Descriptor_< FeatureType, NODENAME >& x,
            const cvx::Descriptor_< FeatureType, NODENAME >& defVal) {
        if (fn.empty()) {
            x = defVal;
        } else {
            x.read(fn, defVal);
        }
    }
}


#endif // CVX_DESCRIPTOR_DESCRIPTOR_HPP__INCLUDED
