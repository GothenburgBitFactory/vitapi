include_directories (${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test) 
add_executable (caps.t caps.t.cpp test.cpp)
target_link_libraries (caps.t libvitapi)
add_executable (color.t color.t.cpp test.cpp)
target_link_libraries (color.t libvitapi)

