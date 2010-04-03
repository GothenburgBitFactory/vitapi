cmake_minimum_required (VERSION 2.8) 
include_directories (${CMAKE_SOURCE_DIR}/src) 
set (vitapi_SRCS Caps.cpp Caps.h
                 Color.cpp Color.h
                 iapi.cpp iapi.h
                 util.cpp util.h
                 vapi.cpp vapi.h)
add_library (libvitapi STATIC ${vitapi_SRCS})
