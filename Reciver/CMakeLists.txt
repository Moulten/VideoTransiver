cmake_minimum_required(VERSION 2.8) 

project( Reciver )

add_library(reciver_lib  STATIC reciver.cpp)

add_executable(Reciver main.cpp)

target_link_libraries( Reciver ${OpenCV_LIBS} reciver_lib)

install(TARGETS Reciver DESTINATION ${CMAKE_SOURCE_DIR}/bin)

