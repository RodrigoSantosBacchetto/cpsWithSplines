/**
 * @file        ContourHierarchyNode.hpp
 *
 * @brief       Defines a class of adaptors for contour hierarchy tree node
 *              inspection.
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

#ifndef CVX_ALGORITHM_CONTOUR_HIERARCHY_NODE_HPP__INCLUDED
#define CVX_ALGORITHM_CONTOUR_HIERARCHY_NODE_HPP__INCLUDED

#pragma once

#include "Imports.hpp"


namespace cvx {


    /**
     * @brief       Adaptor for contour hierarchy tree node inspection.
     */    
    class ContourHierarchyNode : protected Vec4i {

    public:

        ContourHierarchyNode() {
            // NOOP
        }

        ContourHierarchyNode(const Vec4i& h) :
        Vec4i(h) {
            // NOOP
        }

        ContourHierarchyNode& operator=(const ContourHierarchyNode& h) {
            return (Vec4i::operator=(h), *this);
        }

        ContourHierarchyNode& operator=(const Vec4i& h) {
            return (Vec4i::operator=(h), *this);
        }

        int next() const {
            return this->operator[](0);
        }

        int previous() const {
            return this->operator[](1);
        }

        int child() const {
            return this->operator[](2);
        }

        int parent() const {
            return this->operator[](3);
        }

    };


} // namespace cvx


#endif // CVX_ALGORITHM_CONTOUR_HIERARCHY_NODE_HPP__INCLUDED
