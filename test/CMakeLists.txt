include_directories (${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test)
add_custom_target (test ./run_all DEPENDS tapi.t color.t error.t
                                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
add_executable (tapi.t tapi.t.cpp test.cpp)
target_link_libraries (tapi.t vitapi)
add_executable (color.t color.t.cpp test.cpp)
target_link_libraries (color.t vitapi)
add_executable (error.t error.t.cpp test.cpp)
target_link_libraries (error.t vitapi)

configure_file(run_all run_all COPYONLY)
configure_file(problems problems COPYONLY)

