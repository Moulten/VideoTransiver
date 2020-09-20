cmake_minimum_required(VERSION 2.8)

project(ServerCore LANGUAGES CXX)

message("Add ServerCore lib")

add_library(ServerCore STATIC socket.cpp VideoReader.cpp sender.cpp)

message ("Done")

