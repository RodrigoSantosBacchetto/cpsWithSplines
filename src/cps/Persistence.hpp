/**
 * @file       Persistence.hpp
 * @brief      Helpers for OpenCV's XML/YAML persistence mechanism.
 *
 * @author     Waldemar Villamayor-Venialbo <wvenialbo@gmail.com>
 * @version    1.0
 * @date       2015-07-17
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

#ifndef CVX_IO_PERSISTENCE_HPP__INCLUDED
#define CVX_IO_PERSISTENCE_HPP__INCLUDED

#include "Imports.hpp"
#include <opencv2/core/persistence.hpp>


// Macros for OpenCV's XML/YAML Persistence implementation.
// NOTE: These write and read functions must be defined within the
//       `cv` namespace for the serialization in FileStorage to work.


// Forward declaration for class serialization.
#define CVX_DECLARE_CLASS_PERSISTENCE_PROXIES(className) \
    namespace cvx { class className; } \
    namespace cv { \
        void write(FileStorage&, const String&, const cvx::className&); \
        void read(const FileNode&, cvx::className&, const cvx::className&); }


// Definition of proxy methods for class serialization.
#define CVX_DEFINE_CLASS_PERSISTENCE_PROXIES(className) \
    namespace cv { \
        inline void write(FileStorage& fs, const String& name, \
            const cvx::className& x) { x.write(fs, name); } \
        inline void read(const FileNode& fn, cvx::className& x, \
            const cvx::className& defVal) { if (fn.empty()) \
                { x = defVal; } else { x.read(fn, defVal); } } }


// In-class declaration for serialization.
#define CVX_DECLARE_CLASS_PERSISTENCE(className, nodeName) \
    public: \
        void writeTo(FileStorage& fs) const { fs << nodeName << *this; } \
        void writeTo(FileStorage&& fs) const { writeTo(fs); } \
        void save(const std::string& fileName) const { \
            writeTo(FileStorage(fileName, FileStorage::WRITE)); } \
        void readFrom(FileStorage& fs) { fs[nodeName] >> *this; } \
        void readFrom(FileStorage&& fs) { readFrom(fs); } \
        void load(const std::string& fileName) { \
            readFrom(FileStorage(fileName, FileStorage::READ)); } \
    private: \
        void write(FileStorage& fs, const String& name) const; \
        void read(const FileNode& fn, const className& defVal); \
        friend void ::cv::write(FileStorage&, const String&, \
            const className&); \
        friend void ::cv::read(const FileNode&, className&, const className&);


#endif // CVX_IO_PERSISTENCE_HPP__INCLUDED
