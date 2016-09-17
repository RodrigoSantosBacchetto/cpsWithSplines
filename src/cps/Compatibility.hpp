/**
 * @file        Compatibility.hpp
 *
 * @brief       Compatibility issues among compiler implementations.
 *
 * @details     Add standard features lacking by some compilers.
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

#ifndef CVX_UTIL_COMPATIBILITY_HPP__INCLUDED
#define CVX_UTIL_COMPATIBILITY_HPP__INCLUDED

#pragma once


//
// C++ language and ISO standard library features
// lacking by Microsoft Visual Studio C++ compiler.
//

#if defined(_MSC_VER) && !defined(__GNUC__)

#include <ciso646> // MS Compiler compatibility with ISO-646 operators.

#endif // defined(_MSC_VER) && !defined(__GNUC__)


//
// C++ language and ISO standard library features
// lacking by some GNU compiler implementations.
//

#if defined(__GNUC__) and defined(__CYGWIN__)

#if defined(CVX_COMPATIBILITY_USING_TO_STRING)

#include <sstream>
#include <string>

namespace std {

    /**
     * @brief       Conversion to string.
     */
    template < typename T > inline
    string __cvx_to_string(T value) {
        // Create an output string stream, throw the value into the string
        // stream, and convert the string stream into a string and return.
        stringstream ss;
        return (ss << value, ss.str());
    }

}


//
// Conversion to string portability mapping macro.
//
#define to_string __cvx_to_string

#endif // defined(CVX_COMPATIBILITY_USING_TO_STRING)

#endif // defined(__GNUC__) and defined(__CYGWIN__)


#endif // CVX_UTIL_COMPATIBILITY_HPP__INCLUDED
