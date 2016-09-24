cmake_minimum_required(VERSION 3.3)
project(cpsWithSplines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Configuration type
set(CMAKE_CONFIGURATION_TYPES "Debug")

# Source file
SET(SOURCE_FILES
        src/cps/CpSignature.hpp
        src/cps/CpsMatrix.cpp
        src/cps/CpsMatrix.hpp
        src/cps/Imports.hpp
        src/cps/Persistence.hpp
        src/cps/Math.hpp
        src/cps/Signature.hpp
        src/cps/Distance.hpp
        src/cps/NormL2.hpp
        src/cps/DistanceConnective.hpp
        src/cps/NormL1.hpp
        src/cps/DistanceL1.hpp
        src/cps/DistanceLinf.hpp
        src/cps/DistanceL2.hpp
        src/cps/NormL2.hpp
        src/cps/NormLp.hpp
        src/cps/ConnectorFlat.hpp

        # Headers
        # Utility functions
        # Main
        src/main/main.hpp
        src/main/main.cpp)

add_executable(cpsWithSplines ${SOURCE_FILES})

FIND_PACKAGE( OpenCV REQUIRED)

set(OpenCV_FOUND 1)
target_link_libraries(cpsWithSplines ${OpenCV_LIBS} )

include_directories("C:\\eigen-eigen-07105f7124f9")
include_directories("C:\\Users\\Santos\\ClionProjects\\cpsWithSplines\\src\\cps")

