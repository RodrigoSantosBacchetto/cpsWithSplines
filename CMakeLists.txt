cmake_minimum_required(VERSION 3.3)
project(cpsWithSplines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Configuration type
set(CMAKE_CONFIGURATION_TYPES "Debug")

# Source file
SET(SOURCE_FILES
        src/cps/Boundary.cpp
        src/cps/Boundary.hpp
        src/cps/CentroidDistanceFunction.cpp
        src/cps/CentroidDistanceFunction.hpp
        src/cps/ContourHierarchyNode.hpp
        src/cps/CpSignature.hpp
        src/cps/CpsMatrix.cpp
        src/cps/CpsMatrix.hpp
        src/cps/Curve.cpp
        src/cps/Curve.hpp
        src/cps/Compatibility.hpp
        src/cps/Descriptor.hpp
        src/cps/DistanceMatrix.hpp
        src/cps/Imports.hpp
        src/cps/PathBase.hpp
        src/cps/Persistence.hpp
        src/cps/PlanarSet.cpp
        src/cps/PlanarSet.hpp
        src/cps/ShortestPathDistance.hpp
        src/cps/FloydWarshall.hpp
        src/cps/Metric.hpp
        src/cps/PointSet.cpp
        src/cps/PointSet.hpp
        src/cps/Polygon.hpp
        src/cps/TestPoint.hpp
        src/cps/Math.hpp
        src/cps/FreemanChain.cpp
        src/cps/FreemanChain.hpp
        src/cps/PathLength.hpp
        src/cps/Message.cpp
        src/cps/Message.hpp
        src/cps/Signature.hpp
        src/cps/DistanceL2.hpp
        src/cps/EqualLengthSampling.hpp
        src/cps/LineSimplifier.hpp
        src/cps/Distance.hpp
        src/cps/NormL2.hpp
        src/cps/ConnectorFlat.hpp
        src/cps/DistanceConnective.hpp
        src/cps/NormLp.hpp
        src/cps/PolyLine.hpp
        src/cps/Arc.hpp
        src/cps/InnerDistanceL2.hpp
        src/cps/InnerDistanceL2Log.hpp
        src/cps/InnerDistanceL2Squared.hpp
        src/cps/ConnectorL2.hpp
        src/cps/NormL1.hpp
        src/cps/NormL2Log.hpp
        src/cps/NormL2Squared.hpp
        src/cps/NormLinf.hpp
        src/cps/DistanceL1.hpp
        src/cps/DistanceL2Log.hpp
        src/cps/DistanceL2Squared.hpp
        src/cps/DistanceLinf.hpp

        #[[src/cps/AdjacencyMatrix.hpp
        src/cps/ArrayDistance.hpp
        src/cps/BestKeyPoints.hpp
        src/cps/CartesianDistance.hpp
        src/cps/CentroidDistanceHistogram.cpp
        src/cps/CentroidDistanceHistogram.hpp
        src/cps/EigenFeatures.cpp
        src/cps/EigenFeatures.hpp
        src/cps/ShapeSpectrum.hpp
        src/cps/SpectrumDescriptor.cpp
        src/cps/SpectrumDescriptor.hpp
        src/cps/Wavelet.cpp
        src/cps/Mahalanobis.hpp
        src/cps/DistanceMahalanobis.hpp
        src/cps/Wavelet.hpp]]
        # Headers
        # Utility functions
        # Main
        src/main/main.cpp
        )

add_executable(cpsWithSplines ${SOURCE_FILES})

FIND_PACKAGE( OpenCV REQUIRED)

set(OpenCV_FOUND 1)
target_link_libraries(cpsWithSplines ${OpenCV_LIBS} )

include_directories("C:\\eigen-eigen-07105f7124f9")
include_directories("C:\\Users\\Santos\\ClionProjects\\cpsWithSplines\\src\\cps")

