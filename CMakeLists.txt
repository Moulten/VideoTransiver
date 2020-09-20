cmake_minimum_required(VERSION 2.8) 

project( VideoTransiver )

find_package(OpenCV REQUIRED)
if(NOT OPENCV_FOUND)
	message(SEND_ERROR "Failed to find OpenCV")
	return()
else()
	message("include OpenCV")
	include_directories( ${OpenCV_INCLUDE_DIRS} )
endif()

add_subdirectory(Reciver)

add_subdirectory(Server)


