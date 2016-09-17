/**
 * @file        Boundary.cpp
 *
 * @brief       Implements a class of objects that holds information about the
 *              boundary of a planar point set.
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

#define CVX_COMPATIBILITY_USING_TO_STRING 1

#include "Boundary.hpp"
#include "Compatibility.hpp"
#include "ContourHierarchyNode.hpp"
#include <cassert>
#include <string>


namespace cvx {


    Boundary::boundary_t Boundary::cut(const int depth) const {
        int i = 0;
        int current = 0;
        boundary_t cs;
        const hadaptor_t& h = reinterpret_cast< const hadaptor_t& >(_hierarchy);
        CV_Assert(h[0].parent() == -1 and h[0].previous() == -1);
        if (depth == 0) {
            do {
                cs.push_back(_boundary[i]);
                i = h[i].next();
            } while (i >= 0);
            return std::move(cs);
        }
        do {
            const curve_t& c = _boundary[i];
            cs.push_back(c);
            int j = h[i].child();
            if (j >= 0) {
                if (++current > depth) {
                    --current;
                    j = h[i].next();
                    while (j < 0) {
                        if (--current < 0) {
                            break;
                        }
                        i = h[i].parent();
                        j = h[i].next();
                    }
                }
            } else {
                j = h[i].next();
                while (j < 0) {
                    if (--current < 0) {
                        break;
                    }
                    i = h[i].parent();
                    j = h[i].next();
                }
            }
            i = j;
        } while (i >= 0);
        CV_Assert(i < 0 and current == -1);
        return cs;
    }


    Boundary::curve_t Boundary::largest() const {
        curve_t ct;
#if defined(_OPENMP)
        boundary_t bd = slice(0);
        //#pragma omp parallel for
        for (int i = 0; (unsigned)i < bd.size(); ++i) {
            curve_t& cv = bd[i];
            if (cv.size() > ct.size()) {
                // Compare again because `ct` could have
                // been changed by another thread after
                // the comparison outside the critical
                // section.
                //#pragma omp critical
                if (cv.size() > ct.size()) {
                    ct = std::move(cv);
                }
            }
        }
#else
        for (curve_t& cv : slice(0)) {
            if (cv.size() > ct.size()) {
                ct = std::move(cv);
            }
        }
#endif // defined(_OPENMP)
        return ct;
    }


    Boundary::boundary_t Boundary::slice(const int level) const {
        int i = 0;
        int current = 0;
        boundary_t cs;
        const hadaptor_t& h = reinterpret_cast< const hadaptor_t& >(_hierarchy);
        CV_Assert(h[0].parent() == -1 and h[0].previous() == -1);
        if (level == 0) {
            do {
                cs.push_back(_boundary[i]);
                i = h[i].next();
            } while (i >= 0);
            return std::move(cs);
        }
        do {
            const curve_t& c = _boundary[i];
            if (current == level) {
                cs.push_back(c);
            }
            int j = h[i].child();
            if (j >= 0) {
                if (++current > level) {
                    --current;
                    j = h[i].next();
                    while (j < 0) {
                        if (--current < 0) {
                            break;
                        }
                        i = h[i].parent();
                        j = h[i].next();
                    }
                }
            } else {
                j = h[i].next();
                while (j < 0) {
                    if (--current < 0) {
                        break;
                    }
                    i = h[i].parent();
                    j = h[i].next();
                }
            }
            i = j;
        } while (i >= 0);
        CV_Assert(i < 0 and current == -1);
        return cs;
    }


    Boundary Boundary::upsidedown() const {
        // Reduce coordinates for displaying in
        // a right handed coordinate system.
        boundary_t ct = slice(0);
        Point tl = _boundary[0][0], br = tl;
#if defined(_OPENMP)
        //#pragma omp parallel for
        for (int i = 0; (unsigned)i < ct.size(); ++i) {
            const size_t sz = ct[i].size();
            //#pragma omp parallel for firstprivate(i, sz)
            for (int j = 0; (unsigned)j < sz; ++j) {
                const curve_t& cv = ct[i];
                const Point& p = cv[j];
                if (p.x < tl.x) {
                    // Compare again because `tl` could have
                    // been changed by another thread after
                    // the comparison outside the critical
                    // section.
                    //#pragma omp critical
                    if (p.x < tl.x) {
                        tl.x = p.x;
                    }
                } else if (p.x > br.x) {
                    // Compare again because `tl` could have
                    // been changed by another thread after
                    // the comparison outside the critical
                    // section.
                    //#pragma omp critical
                    if (p.x > br.x) {
                        br.x = p.x;
                    }
                }
                if (p.y < tl.y) {
                    // Compare again because `tl` could have
                    // been changed by another thread after
                    // the comparison outside the critical
                    // section.
                    //#pragma omp critical
                    if (p.y < tl.y) {
                        tl.y = p.y;
                    }
                } else if (p.y > br.y) {
                    // Compare again because `tl` could have
                    // been changed by another thread after
                    // the comparison outside the critical
                    // section.
                    //#pragma omp critical
                    if (p.y > br.y) {
                        br.y = p.y;
                    }
                }
            }
        }
#else
        for (const curve_t& cv : ct) {
            for (const Point& p : cv) {
                if (p.x < tl.x) {
                    tl.x = p.x;
                } else if (p.x > br.x) {
                    br.x = p.x;
                }
                if (p.y < tl.y) {
                    tl.y = p.y;
                } else if (p.y > br.y) {
                    br.y = p.y;
                }
            }
        }
#endif // defined(_OPENMP)
        ct = _boundary;
#if defined(_OPENMP)
        //#pragma omp parallel for
        for (int i = 0; (unsigned)i < ct.size(); ++i) {
            const size_t sz = ct[i].size();
            //#pragma omp parallel for firstprivate(i, sz)
            for (int j = 0; (unsigned)j < sz; ++j) {
                curve_t& cv = ct[i];
                Point& p = cv[j];
#else
        for (curve_t& cv : ct) {
            for (Point& p : cv) {
#endif // defined(_OPENMP)
                p.x = p.x - tl.x;
                p.y = br.y - p.y;
            }
        }
        return Boundary(std::move(ct), hierarchy_t(_hierarchy));
    }


    /**
     * @brief       Implements OpenCV XML/YAML serialization.
     * @param[in]   fs Reference to a `FileStorage` object.
     * @param[in]   name Reference to root element name.
     */
    void Boundary::write(FileStorage& fs, const String&) const {
        CV_Assert(_boundary.size() > 0                      // non-empty.
            and _boundary.size() == _hierarchy.size()); // data coherence.
        // Serialize silhouette data.
        fs << "{"
                << _node[COLLECTION_SIZE] << (int) _boundary.size()
                << _node[CONTOURS_LIST] << "{";
        for (unsigned i = 0; i < _boundary.size(); ++i) {
            fs << _node[CONTOUR_ITEM] + std::to_string(i) << data(i);
        }
        fs << "}"
                << _node[CONTOURS_TREE] << "{";
        for (unsigned i = 0; i < _hierarchy.size(); ++i) {
            fs << _node[NODE_ITEM] + std::to_string(i) << _hierarchy[i];
        }
        fs << "}" << "}";
    }


    /**
     * @brief       Implements OpenCV XML/YAML deserialization.
     * @param[in]   fs Reference to a `FileStorage` object.
     */
    void Boundary::read(const FileNode& fn, const Boundary&) {
        // Deserialize the silhouette data.
        // Delay assigment to avoid error due zombie state.
        const size_t csize = static_cast< size_t > ((int) fn[_node[COLLECTION_SIZE]]);
        boundary_t boundary;
        for (auto item : fn[_node[CONTOURS_LIST]]) {
            curve_t ctr;
            item >> ctr;
            boundary.push_back(ctr);
        }
        hierarchy_t hierarchy;
        for (auto item : fn[_node[CONTOURS_TREE]]) {
            hierarchy_t::value_type node;
            item >> node;
            hierarchy.push_back(node);
        }
        CV_Assert(csize > 0 and boundary.size() == csize // non-empty.
            and boundary.size() == hierarchy.size()); // data coherence.
        set(std::move(boundary), std::move(hierarchy));
    }

    const char* const Boundary::_node[5] = {
        "contours-list",
        "contours-tree",
        "contour_",
        "node_",
        "size"
    };


} // namespace cvx
