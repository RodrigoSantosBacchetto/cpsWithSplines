cmake_minimum_required(VERSION 3.3)
project(cpsWithSplines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

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
        src/main/cpsFunctions.hpp
        src/main/drawUtilityClasses.hpp
        src/main/filesManagementFunctions.hpp
        src/main/generalFunctions.hpp
        src/main/contourUtilities.hpp
        src/main/noiseFunctions.hpp
        src/main/main.cpp src/experiments/bigExperimentForTresholdAndErrorRate.hpp)

add_executable(cpsWithSplines ${SOURCE_FILES})

FIND_PACKAGE( OpenCV REQUIRED)

set(OpenCV_FOUND 1)
target_link_libraries(cpsWithSplines ${OpenCV_LIBS} )

include_directories("D:\\FP-UNA\\eigen-eigen-07105f7124f9\\eigen-eigen-07105f7124f9")
include_directories("D:\\FP-UNA\\opencvandtools\\cpsWithSplines\\src\\cps")

