/* 
 * @file        FreemanChain.hpp
 *
 * @brief       Defines an utility class for encoding/decoding Freeman's
 *              chain codes.
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

#ifndef CVX_ALGORITHM_FREEMAN_CHAIN_HPP__INCLUDED
#define CVX_ALGORITHM_FREEMAN_CHAIN_HPP__INCLUDED

#pragma once

#include "Imports.hpp"


namespace cvx {


    /**
     * @brief   Freeman chain encoding/decoding utility class.
     */
    class FreemanChain {

    public:

        typedef Vector2i path_t;  /**< A path of 2D points. */

        typedef Vector1i chain_t; /**< Freeman chain. */


    public:

        /**
            * @brief   Returns a vector holding the Freeman chain-code for this
            *          path.
            *
            * @return  A `std::vector< int >` containing the Freeman chain-code for
            *          this path.
            */
        static
        chain_t encode(const path_t& path);


        /**
            * @brief       Decode a Freeman chain-code.
            *
            * @param[in]   chain A reference to a `std::vector< int >` containing
            *              a Freeman chain-code.
            *
            * @return      A `std::vector< Point >` with the decoded path.
            */
        static
        path_t decode(const chain_t& chain);

    };


} // namespace cvx


#endif // CVX_ALGORITHM_FREEMAN_CHAIN_HPP__INCLUDED
