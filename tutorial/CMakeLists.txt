cmake_minimum_required(VERSION 3.0)

project(tutorial)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_search_module(GST REQUIRED gstreamer-1.0)
pkg_search_module(GSTAUDIO REQUIRED gstreamer-audio-1.0)
pkg_search_module(GSTPBUTILS REQUIRED gstreamer-pbutils-1.0)

include_directories(${GST_INCLUDE_DIRS}
        ${GSTAUDIO_INCLUDE_DIRS} ${GSTPBUTILS_INCLUDE_DIRS})
# add_definitions(-D__STDC_CONSTANT_MACROS)
link_directories(${GST_LIBRARY_DIRS}
        ${GSTAUDIO_LIBRARY_DIRS} ${GSTPBUTILS_LIBRARY_DIRS})

add_executable(basic-tutorial-1 basic-tutorial-1.c)
target_link_libraries(basic-tutorial-1 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-1 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-2 basic-tutorial-2.c)
target_link_libraries(basic-tutorial-2 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-2 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-3 basic-tutorial-3.c)
target_link_libraries(basic-tutorial-3 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-3 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-4 basic-tutorial-4.c)
target_link_libraries(basic-tutorial-4 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-4 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-6 basic-tutorial-6.c)
target_link_libraries(basic-tutorial-6 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-6 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-7 basic-tutorial-7.c)
target_link_libraries(basic-tutorial-7 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-7 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-8 basic-tutorial-8.c)
target_link_libraries(basic-tutorial-8 ${GST_LIBRARIES} ${GSTAUDIO_LIBRARIES})
target_compile_options(basic-tutorial-8 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-9 basic-tutorial-9.c)
target_link_libraries(basic-tutorial-9 ${GST_LIBRARIES} ${GSTPBUTILS_LIBRARIES})
target_compile_options(basic-tutorial-9 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-12 basic-tutorial-12.c)
target_link_libraries(basic-tutorial-12 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-12 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})

add_executable(basic-tutorial-13 basic-tutorial-13.c)
target_link_libraries(basic-tutorial-13 ${GST_LIBRARIES})
target_compile_options(basic-tutorial-13 PUBLIC ${GST_CFLAGS} ${GST_CFLAGS_OTHER})
