cmake_minimum_required(VERSION 3.21)
project(point_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES src/parser.cpp src/serializer.cpp)
add_executable(task main.cpp ${SOURCES})

target_include_directories(task PUBLIC "${PROJECT_SOURCE_DIR}/src")