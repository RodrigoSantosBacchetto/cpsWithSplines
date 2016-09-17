/**
 * @file       Message.cpp
 * @brief      Implements library wide messages.
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

#include "Message.hpp"


namespace cvx {

    const char* const Message::_messages[1024] = {
        "Non suitable sampling size for the Discrete Wavelet Transform.",
        "Bad chain code. Possibly corrupted contour data.",
        "The ChainCode adaptor is not suitable for resampled contours."
    };

} // namespace cvx
