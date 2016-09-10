cmake_minimum_required(VERSION 3.3)
project(cpsWithSplines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Configuration type
set(CMAKE_CONFIGURATION_TYPES "Debug")

#[[# Headers
SET(SOURCE_FILES
        src/cps/CpsMatrix.hpp
        src/cps/Imports.hpp
        src/cps/Persistence.hpp
        )

# Utility functions
SET(SOURCE_FILES
        src/cps/CpsMatrix.cpp
        )]]

# Source file
SET(SOURCE_FILES
        src/main/main.cpp
        )

add_executable(cpsWithSplines ${SOURCE_FILES})

FIND_PACKAGE( OpenCV REQUIRED)

set(OpenCV_FOUND 1)
target_link_libraries(cpsWithSplines ${OpenCV_LIBS} )

find_package (vector)

include_directories("C:\\eigen-eigen-07105f7124f9")

