cmake_minimum_required (VERSION 2.6)
project (LogScanner C CXX)

SET_DIRECTORY_PROPERTIES(PROPERTIES 
   ADDITIONAL_MAKE_CLEAN_FILES "cmake_install.cmake;CMakeCache.txt;Makefile") 

set (LogScanner_VERSION_MAJOR 0)
set (LogScanner_VERSION_MINOR 0)
set (LogScanner_VERSION_PATCH 0)

set (CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set (CMAKE_CXX_FLAGS "-std=c++11 -pthread")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Threads
find_package (Threads REQUIRED)
set (LIBS ${LIBS} ${Threads_LIBRARIES})

# glog
find_package (Glog REQUIRED)
set (LIBS ${LIBS} ${GLOG_LIBRARIES})

# GFlags
set (gflags_SHARED    FALSE)
set (gflags_NOTHREADS FALSE)
find_package(gflags REQUIRED)
set (LIBS ${LIBS} gflags)

# Unwind
find_package (Unwind REQUIRED)
set (LIBS ${LIBS} ${UNWIND_LIBRARIES} unwind)

# JSONCpp
find_package (JsonCpp REQUIRED)
set (LIBS ${LIBS} ${JSONCPP_LIBRARIES})

add_library (filescanner STATIC src/FileScanner.cpp src/FileScanner.h)
add_executable (logscanner src/logscanner.cpp)

target_link_libraries (logscanner filescanner ${LIBS})
