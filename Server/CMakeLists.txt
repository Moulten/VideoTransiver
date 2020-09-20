cmake_minimum_required(VERSION 2.8)

project( Server LANGUAGES CXX)

SET(PLATFORM_SPECIFIC_LIBS "-lpthread")

add_executable( Server main.cpp )

include_directories(core)

add_subdirectory(core)

target_link_libraries( Server ${OpenCV_LIBS} 
			${PLATFORM_SPECIFIC_LIBS} ServerCore)

install(TARGETS Server DESTINATION ${CMAKE_SOURCE_DIR}/bin)

