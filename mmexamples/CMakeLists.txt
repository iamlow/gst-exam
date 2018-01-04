cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(mmexamples)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
# pkg_search_module(GST REQUIRED gstreamer-1.0)
pkg_search_module(GSTMM REQUIRED gstreamermm-1.0)

include_directories(${GSTMM_INCLUDE_DIRS})
# add_definitions(-D__STDC_CONSTANT_MACROS)
link_directories(${GSTMM_LIBRARY_DIRS})

add_executable(hello_world hello_world.cc)
target_link_libraries(hello_world ${GSTMM_LIBRARIES})
target_compile_options(hello_world PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(bin bin.cc)
target_link_libraries(bin ${GSTMM_LIBRARIES})
target_compile_options(bin PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})
