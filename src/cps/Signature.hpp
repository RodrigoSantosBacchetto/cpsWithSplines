/**
 * @file        Signature.hpp
 *
 * @brief       Defines a (discrete) signature (real descriptor) metaclass.
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

#ifndef CVX_DESCRIPTOR_SIGNATURE_HPP__INCLUDED
#define CVX_DESCRIPTOR_SIGNATURE_HPP__INCLUDED

#pragma once

#include "Imports.hpp"
#include "Descriptor.hpp"


namespace cvx {


    template< const char* const NODENAME >
    class Signature_ : public Descriptor_< Real, NODENAME > {

    private:

        typedef Descriptor_< Real, NODENAME > base_type;


    public:

        using typename base_type::vector_t;


    public:

        /**
         * @brief      Default constructor.
         */
        Signature_() :
        base_type() {
            // NOOP
        }


        /**
         * @brief       Copy constructor.
         * @param[in]   src A constant reference to a source object.
         */
        Signature_(const Signature_& src) :
        base_type(src) {
            // NOOP
        }


        /**
         * @brief           Move constructor.
         * @param[in,out]   tmp A rvalue reference to a source object.
         */
        Signature_(Signature_&& tmp) :
        base_type(std::move(tmp)) {
            // NOOP
        }


        /**
         * @brief       Clone constructor.
         * @param[in]   src A constant reference to a compatible source object.
         */
        template< typename OtherFeatureType >
        Signature_(const Descriptor_< OtherFeatureType, NODENAME >& src) :
        base_type(src) {
            // NOOP
        }


        /**
         * @brief       Copy wrapper constructor.
         * @param[in]   data A constant reference to a data vector.
         */
        Signature_(const vector_t& data) :
        base_type(data) {
            // NOOP
        }


        /**
         * @brief           Move wrapper constructor.
         * @param[in,out]   data A rvalue reference to a data vector.
         */
        Signature_(vector_t&& data) :
        base_type(std::move(data)) {
            // NOOP
        }


        /**
         * @brief      Destructor.
         */
        ~Signature_() {
            // NOOP
        }

    };


} // namespace cvx


#endif // CVX_DESCRIPTOR_SIGNATURE_HPP__INCLUDED
