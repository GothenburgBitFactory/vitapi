cmake_minimum_required (VERSION 2.8) 
include_directories (${CMAKE_SOURCE_DIR}/src) 
set (vitapi_SRCS color.cpp
                 iapi.cpp
                 vapi.cpp
                 tapi.cpp
                 util.cpp util.h
                 error.cpp
                 vitapi.h)
add_library (vitapi STATIC ${vitapi_SRCS})
