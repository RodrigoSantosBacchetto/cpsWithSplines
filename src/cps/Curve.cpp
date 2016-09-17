/**
 * @file        Curve.cpp
 *
 * @brief       Implements a class of objects that hold information about a
 *              closed path line, e.g., a discrete curve or the vertices of
 *              a polygon.
 *
 * @brief       Defines a class that holds information about a (closed) curve,
 *              e.g., the contour of an object.
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

#include "Curve.hpp"
#include "Compatibility.hpp"
#include "TestPoint.hpp"
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <string>
#include <iomanip>


namespace cvx {

    namespace util {

        bool getComment(std::istream& s) {

            // Consumme comment lines.
            char c = '#';

            while (c == '#') {
                if (!(s >> c)) {
                    return false;
                }
                if (c == '#') {
                    std::string rem;
                    if (!std::getline(s, rem)) {
                        return false;
                    }
                } else {
                    s.putback(c);
                    c = 0;
                }
            }
            return true;
        }

    }


    void Curve::ctread(const std::string& fileName) {

        // Stage 1) Load the source contour file.
        path_t contour;
        // ·························································
        //CVX_TEST_POINT;

        // Step 1.1) Try open the source file.
        std::ifstream s(fileName.c_str(), std::ios::binary);
        if (not s.is_open()) {
            throw std::runtime_error("File access error: Unable to open the "
                                     "source file");
        }
        // ---------------------------------------------------------
        //CVX_TEST_POINT;

        // Step 1.2) Try read the header from the file.
        std::string hash;
        if (!(s >> hash)) {
            throw std::runtime_error("File access error: Unable to read the "
                                     "source file header");
        }
        // ---------------------------------------------------------
        //CVX_TEST_POINT;

        // Step 1.3) Check validity and identify the file format.
        static const char* const CPSX_FILE_SIG_CTR = "#";
        static const char* const CPSX_FILE_SIG_CTA = "CTA";
        enum {
            SIMPLE_CTR,
            EXTENDED_A_CTX,
        };
        int type = -1;
        if (hash == CPSX_FILE_SIG_CTR) {
            // Loads a F. Mokhtarian's contour data file.
            type = SIMPLE_CTR;
        } else if (hash == CPSX_FILE_SIG_CTA) {
            // Loads propietary contour data file (ASCII).
            type = EXTENDED_A_CTX;
        } else {
            throw std::logic_error("File format error: Invalid source file "
                                   "signature");
        }
        // ---------------------------------------------------------
        //CVX_TEST_POINT;

        // Step 1.4) Retrieve silhouette info.
        int vertnum = 0;
        if (type == SIMPLE_CTR) {
            if (!(s >> vertnum)) {
                throw std::runtime_error("File access error: Unable to read "
                                         "the source file info");
            }
        } else {
            std::string token;
            do {
                // Consumme any comment.
                if (!util::getComment(s) or !(s >> token)) {
                    throw std::runtime_error("File access error: Unexpected "
                                             "EOF");
                }
                // NOTE: These data are discarded, but checked, in this
                // implementation, these data were from the Contour-Point
                // Signature project.
                if (token == "VERTNUM") {
                    if (!(s >> vertnum)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                    CV_Assert(vertnum > 0);
                } else if (token == "RECT") {
                    Point tl, br;
                    if (!(s >> tl.y >> br.x >> br.y >> tl.x)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "AREA") {
                    Real area;
                    if (!(s >> area)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "PERIMETER") {
                    Real perimeter;
                    if (!(s >> perimeter)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "CENTROID") {
                    Point2r cd;
                    if (!(s >> cd.x >> cd.y)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "VARIANCE") {
                    Real vx, vy, vxy;
                    if (!(s >> vx >> vy >> vxy)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "AXIS") {
                    Real cos_th, sin_th;
                    if (!(s >> cos_th >> sin_th)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "INTERSECTX") {
                    Point2r ll, lr, rl, rr;
                    if (!(s >> ll.x >> ll.y >> lr.x >> lr.y
                            >> rl.x >> rl.y >> rr.x >> rr.y)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "INTERSECTY") {
                    Point2r bb, bt, tb, tt;
                    if (!(s >> bb.x >> bb.y >> bt.x >> bt.y
                            >> tb.x >> tb.y >> tt.x >> tt.y)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else if (token == "ECCENTR") {
                    Real e;
                    if (!(s >> e)) {
                        throw std::runtime_error("File access error: "
                                                 "Unexpected EOF");
                    }
                } else {
                    if (token != "ENDHDR") {
                        throw std::logic_error("Unrecognized token \""
                            + token + '\"');
                    }
                }
            }
            while (token != "ENDHDR");
            char c;
            if (!(s >> c)) {
                throw std::runtime_error("File access error: Unexpected EOF");
            }
            if (c != '\n') {
                s.putback(c);
            }
        }
        // ---------------------------------------------------------
        //CVX_TEST_POINT << vertnum << "\n";

        // Step 1.5) Gather contour's vertices.
        if (type == SIMPLE_CTR or type == EXTENDED_A_CTX) {
            Point p;
            for (int i = 0; i < vertnum; ++i) {
                if (!(s >> p.x >> p.y)) {
                    throw std::runtime_error("Unexpected EOF while reading "
                                             "contour data");
                }
                contour.push_back(p);
            }
            if (contour.size() != vertnum or vertnum == 0) {
                throw std::logic_error("Corrupted contour data or empty "
                                       "contour");
            }
        }
        CV_Assert(contour.size() == vertnum);
        // =========================================================
        //CVX_TEST_POINT << contour.size() << "\n";


        // Stage 2) Setup this Curve object.
        // ·························································
        *this = Curve(contour);
        // =========================================================

    }


    bool Curve::ctwrite(const std::string& fileName) const {

        const path_t& contour = data();

        if (contour.size() == 0) {
            throw std::logic_error("Empty contour");
        }

        static const char* const CPSX_FILE_SIG_CTR = "#";
        static const char* const CPSX_FILE_SIG_CTA = "CTA";
        enum {
            SIMPLE_CTR,
            EXTENDED_A_CTX,
        };
        int mode = -1;
        size_t pos = fileName.find_last_of('.');
        if (pos == std::string::npos) {
            throw std::logic_error("Invalid filename");
        } else {
            std::string ext = fileName.substr(pos);
            if (ext == ".ctr") {
                mode = EXTENDED_A_CTX;
            } else if (ext == ".c") {
                mode = SIMPLE_CTR;
            } else {
                throw std::logic_error("Unecognized output file format");
            }
        }

        std::ofstream out(fileName.c_str(), std::ios::binary);
        if (not out.is_open()) {
            throw std::runtime_error("File access error: Unable to create "
                                     "the output file");
        }

        if (mode == SIMPLE_CTR) {
            // Write # of vertices.
            out << CPSX_FILE_SIG_CTR << " "
                << (unsigned)contour.size() << "\n"
                    "# F. Mokhtarian's contour data file format\n"
                    "# S-Spectra Project (C) 2015 by WVV\n";
        } else if (mode == EXTENDED_A_CTX) {
            // Write file signature.
            out << CPSX_FILE_SIG_CTA << "\n"
                    "# Propietary contour data file format\n"
                    "# S-Spectra Project (C) 2015 by WVV\n";
            out << std::setprecision(15) << std::scientific;
            const Rect bd = boundingRect(contour);
            const Real area = contourArea(contour);
            const Real perimeter = length();
            const Moments m = moments(contour);
            // Write # of vertices.
            out << "VERTNUM " << (unsigned)contour.size() << '\n'
            // Write bounding rectangle.
                << "RECT " << bd.tl().y << ' '
                           << bd.br().x << ' '
                           << bd.br().y << ' '
                           << bd.tl().x << '\n'
            // Write area, length, and centroid cordinates.
                << "AREA " << area << '\n'
                << "PERIMETER " << perimeter << '\n'
                << "CENTROID " << m.m10 / m.m00 << ' '
                               << m.m01 / m.m00 << '\n'
            // Write header terminator.
                << "ENDHDR\n";
        }

        if (!out) {
            throw std::runtime_error("File access error: Unable to write the "
                                     "output file");
        }

        if (mode == SIMPLE_CTR or mode == EXTENDED_A_CTX)
        {
            // Write list of vertex coordinates in ASCII mode.
            for (const Point& p : contour) {
                if (!(out << p.x << ' ' << p.y << '\n')) {
                    throw std::runtime_error("File access error: Unable to "
                                             "write the output file");
                }
            }
        }

        return out != nullptr;
    }


} // namespace cvx
