cmake_minimum_required(VERSION 3.1)
project(make_spoof)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(utils)
add_subdirectory(internal_thread)
add_subdirectory(unit)
add_executable(capture_spoof
        capture_spoof.cc
        tools/spoof_capture.cc
        )
add_executable(align_spoof
        align_spoof.cc
        tools/spoof_align.cc)
target_link_libraries(capture_spoof unit internal_thread utils ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} X11)
target_link_libraries(align_spoof unit internal_thread utils ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} tesseract)