cmake_minimum_required(VERSION 3.3)
project(tesis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Configuration type
set(CMAKE_CONFIGURATION_TYPES "Debug")

# Source directory
include_directories(source-files)

# Local source files
SET(SOURCE_FILES
                lastCode.cpp
)

add_executable(tesis ${SOURCE_FILES})

FIND_PACKAGE( OpenCV REQUIRED)

set(OpenCV_FOUND 1)
target_link_libraries(tesis ${OpenCV_LIBS} )

find_package (vector)

include_directories("C:\\eigen-eigen-07105f7124f9")

