cmake_minimum_required(VERSION 2.8) 

project( sender )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(sender sender.cpp)

target_link_libraries( sender ${OpenCV_LIBS})

