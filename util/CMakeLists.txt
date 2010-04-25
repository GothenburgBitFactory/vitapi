cmake_minimum_required (VERSION 2.8)
set (UTIL_PRGS termdef color)
add_custom_target (util DEPENDS ${UTIL_PRGS})
include_directories (${CMAKE_SOURCE_DIR}/src) 
add_executable (termdef termdef.c)
target_link_libraries(termdef ncurses)
add_executable (color color.c)
target_link_libraries(color vitapi)

