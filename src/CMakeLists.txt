cmake_minimum_required (VERSION 2.8) 
include_directories (${CMAKE_SOURCE_DIR}/src) 
set (vitapi_SRCS color.cpp
                 iapi.cpp
                 vapi.cpp
                 tapi.cpp
                 util.cpp util.h
                 error.cpp
                 vitapi.h
                 check.h)
add_library (vitapi STATIC ${vitapi_SRCS})
set (CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")
install (TARGETS vitapi DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES vitapi.h DESTINATION include)

set (CMAKE_BUILD_TYPE debug)
set (CMAKE_C_FLAGS_DEBUG "-ggdb3")
set (CMAKE_C_FLAGS_RELEASE "-O3")

