/**
 * @file        PointSet.cpp
 *
 * @brief       Implements a class of objects that represent sparse planar
 *              point sets.
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

#include "Compatibility.hpp"
#include "PointSet.hpp"
#include <cassert>


namespace cvx {


    const char* const PointSet::_node[2] = {
        "size",
        "points"
    };


    /**
     * @brief           Implements OpenCV XML/YAML serialization.
     *
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     *
     * @param[in]       name  Reference to root element name.
     */
    void PointSet::write(FileStorage& fs, const String&) const {
        // Serialize the data.
        fs << '{'
            << _node[SIZE] << (int) size()
            << _node[POINTS] << data()
            << '}';
    }


    /**
     * @brief           Implements OpenCV XML/YAML deserialization.
     *
     * @param[in,out]   fs  Reference to a `FileStorage` object.
     */
    void PointSet::read(const FileNode& fn, const PointSet&) {
        // Deserialize the data.
        fn[_node[POINTS]] >> _set;
    }


} // namespace cvx
