cmake_minimum_required(VERSION 3.16)
project(PID)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(testing)

add_executable(PID
        testing/analysis.cpp
        testing/analysis.h
        testing/main.cpp
        testing/tune.cpp
        pid.c
        pid.h)

set(COMPONENT_ADD_INCLUDEDIRS .)

set(COMPONENT_SRCS "pid.c")

set(COMPONENT_REQUIRES)

register_component()
