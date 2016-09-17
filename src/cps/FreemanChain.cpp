/* 
 * @file        FreemanChain.cpp
 *
 * @brief       Implements an utility class for encoding/decoding Freeman's
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

#include "Compatibility.hpp"
#include "FreemanChain.hpp"
#include "Message.hpp"


namespace cvx {

    FreemanChain::chain_t FreemanChain::encode(const path_t& path) {
        // Compute the 8-connected Freeman chain code.
        //
        // Direction-to-code convention is:
        //
        //      3  2  1     0 (+1,  0)      3 (-1, +1)      6 ( 0, -1)
        //      4  x  0     1 (+1, +1)      4 (-1,  0)      7 (+1, -1)
        //      5  6  7     2 ( 0, +1)      5 (-1, -1)
        //
        // In terms of (delta_x, delta_y) if next pixel compared to the
        // current and converting (dy,dx) pairs to scalar indexes thinking
        // to them as base-3 numbers according to:
        //
        //      i = 3 * (dy+1) + (dx+1) = 3dy + dx + 4
        //
        //      ---------------------------------------
        //      | deltax | deltay | code |  (base-3)  |
        //      |------------------------------------|
        //      |    0   |   +1   |   2  |      7     | 
        //      |    0   |   -1   |   6  |      1     | 
        //      |   -1   |   +1   |   3  |      6     | 
        //      |   -1   |   -1   |   5  |      0     | 
        //      |   +1   |   +1   |   1  |      8     | 
        //      |   +1   |   -1   |   7  |      2     | 
        //      |   -1   |    0   |   4  |      3     |  
        //      |   +1   |    0   |   0  |      5     | 
        //      ---------------------------------------
        //
        static const int cc[3][3] = {
            {+5, +6, +7},
            {+4, -1, +0},
            {+3, +2, +1}
        };
        chain_t chain;
        Point i;
        // Traverse the vector to obtain the chain codes.
        for (const auto& j : path) {
            if (chain.size() == 0) {
                // Store the (x,y) coordinates of the starting point.
                chain.push_back(j.x);
                chain.push_back(j.y);
            } else {
                const int dx = j.x - i.x + 1,
                    dy = j.y - i.y + 1;
                if (0 <= dx and dx <= 2 and 0 <= dy and dy <= 2) {
                    const int code = cc[dy][dx];
                    if (code < 0) {
                        CVX_THROW(std::domain_error, BAD_CHAIN_CODE);
                    }
                    // Store the current point Freeman code.
                    chain.push_back(code);
                } else {
                    CVX_THROW(std::invalid_argument, NON_SUITABLE_CHAIN);
                }
            }
            i = j;
        }
        return chain;
    }


    /**
    * @brief       Decode a Freeman chain-code.
    *
    * @param[in]   chain A reference to a `std::vector< int >` containing
    *              a Freeman chain-code.
    *
    * @return      A `std::vector< Point >` with the decoded path.
    */
    FreemanChain::path_t FreemanChain::decode(const chain_t& chain) {
        // Traverse the Freeman codes chain and reconstruct the path.
        static bool pp_init = true;
        static Point pp[8];
        if (pp_init) {
            pp_init = false;
            pp[0] = Point(+1, +0);
            pp[1] = Point(+1, +1);
            pp[2] = Point(+0, +1);
            pp[3] = Point(-1, +1);
            pp[4] = Point(-1, +0);
            pp[5] = Point(-1, -1);
            pp[6] = Point(+0, -1);
            pp[7] = Point(+1, -1);
        }
        path_t path;
        // Set the path starting point.
        Point p, q;
        auto it = chain.begin();
        if (it != chain.end()) {
            p.x = *it++;
        }
        if (it != chain.end()) {
            p.y = *it++;
        }
        path.push_back(p);
        while (it != chain.end()) {
            p += pp[*it++];
            path.push_back(p);
        }
        return path;
    }


} // namespace cvx
