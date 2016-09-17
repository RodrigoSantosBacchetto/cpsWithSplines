/**
 * @file       Message.hpp
 * @brief      Library wide messages.
 *
 * @author     Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 * @version    1.0
 * @date       2015-03-20
 *
 * @copyright  Copyright © 2015, National University of Asunción, all rights
 *             reserved.
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

#ifndef CVX_UTIL_MESSAGE_HPP__INCLUDED
#define CVX_UTIL_MESSAGE_HPP__INCLUDED

#pragma once


#include <string>
#include <stdexcept>


namespace cvx {

    /**
     * @brief   Library messages utility class.
     */
    class Message {
    public:

        enum index_t {
            NON_SUITABLE_DWT,
            BAD_CHAIN_CODE,
            NON_SUITABLE_CHAIN
        };

        static std::string get(const index_t i) {
            return _messages[i];
        }

    private:

        static const char* const _messages[1024];

    };

} // namespace cvx


#define CVX_THROW(ex, msg) \
    throw ex(cvx::Message::get(cvx::Message::msg))

#endif	// CVX_UTIL_MESSAGE_HPP__INCLUDED
