cmake_minimum_required(VERSION 3.0)

project(examples)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_search_module(GST REQUIRED gstreamer-1.0)

include_directories(${GST_INCLUDE_DIRS})
# add_definitions(-D__STDC_CONSTANT_MACROS)
link_directories(${GST_LIBRARY_DIRS})

add_executable(ex4_1 ex4_1.c)
target_link_libraries(ex4_1 ${GST_LIBRARIES})
target_compile_options(ex4_1 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(ex4_2 ex4_2.c)
target_link_libraries(ex4_2 ${GST_LIBRARIES})
target_compile_options(ex4_2 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(ex5_2a ex5_2a.c)
target_link_libraries(ex5_2a ${GST_LIBRARIES})
target_compile_options(ex5_2a PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(ex5_3 ex5_3.c)
target_link_libraries(ex5_3 ${GST_LIBRARIES})
target_compile_options(ex5_3 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(ex5_4_1 ex5_4_1.c)
target_link_libraries(ex5_4_1 ${GST_LIBRARIES})
target_compile_options(ex5_4_1 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(ex6_2 ex6_2.c)
target_link_libraries(ex6_2 ${GST_LIBRARIES})
target_compile_options(ex6_2 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(ex10_1 ex10_1.c)
target_link_libraries(ex10_1 ${GST_LIBRARIES})
target_compile_options(ex10_1 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})
