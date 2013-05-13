cmake_minimum_required (VERSION 2.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required

project (vitapi)

add_subdirectory (src)
add_subdirectory (examples EXCLUDE_FROM_ALL)
add_subdirectory (test EXCLUDE_FROM_ALL)
add_subdirectory (util EXCLUDE_FROM_ALL)
