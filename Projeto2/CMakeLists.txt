cmake_minimum_required(VERSION 3.16)
project(imgfile-example)

#SET (CMAKE_BUILD_TYPE "Release")
SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/bin)

find_package(OpenCV REQUIRED) 

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(ppm_extract STATIC ppm_extract.cpp)
add_executable (mainExtract mainExtract.cpp)
target_link_libraries(mainExtract ppm_extract ${OpenCV_LIBS})

add_library(img_tools STATIC img_tools.cpp)
add_executable (mainTools mainTools.cpp)
target_link_libraries(mainTools img_tools ${OpenCV_LIBS})


add_library(golomb STATIC golomb.cpp)

add_library(bitstream STATIC bitstream.cpp)

add_library(sndCodec STATIC sndCodec.cpp)

add_executable(sndCodecMain sndCodecMain.cpp)
target_link_libraries(sndCodecMain sndCodec golomb bitstream sndfile)

#DEBUG
add_executable(testbitstream testbitstream.cpp) 
target_link_libraries(testbitstream bitstream)      
