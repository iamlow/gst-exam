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

add_executable(bus bus.cc)
target_link_libraries(bus ${GSTMM_LIBRARIES})
target_compile_options(bus PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(dynamic_pads dynamic_pads.cc)
target_link_libraries(dynamic_pads ${GSTMM_LIBRARIES})
target_compile_options(dynamic_pads PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(element_factory element_factory.cc)
target_link_libraries(element_factory ${GSTMM_LIBRARIES})
target_compile_options(element_factory PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(init_gstreamermm init_gstreamermm.cc)
target_link_libraries(init_gstreamermm ${GSTMM_LIBRARIES})
target_compile_options(init_gstreamermm PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(all_media_player all_media_player.cc)
target_link_libraries(all_media_player ${GSTMM_LIBRARIES})
target_compile_options(all_media_player PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(audio_video_muxer audio_video_muxer.cc)
target_link_libraries(audio_video_muxer ${GSTMM_LIBRARIES})
target_compile_options(audio_video_muxer PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(dynamic_changing_element dynamic_changing_element.cc)
target_link_libraries(dynamic_changing_element ${GSTMM_LIBRARIES})
target_compile_options(dynamic_changing_element PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(dynamic_changing_source dynamic_changing_source.cc)
target_link_libraries(dynamic_changing_source ${GSTMM_LIBRARIES})
target_compile_options(dynamic_changing_source PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(element_link element_link.cc)
target_link_libraries(element_link ${GSTMM_LIBRARIES})
target_compile_options(element_link PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(tee_request_pad tee_request_pad.cc)
target_link_libraries(tee_request_pad ${GSTMM_LIBRARIES})
target_compile_options(tee_request_pad PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(typefind typefind.cc)
target_link_libraries(typefind ${GSTMM_LIBRARIES})
target_compile_options(typefind PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})

add_executable(nvcam_rtmp nvcam_rtmp.cc)
target_link_libraries(nvcam_rtmp ${GSTMM_LIBRARIES})
target_compile_options(nvcam_rtmp PUBLIC ${GSTMM_CFLAGS} ${GSTMM_CFLAGS_OTHER})
