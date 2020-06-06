cmake_minimum_required(VERSION 3.16)
project(PID)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(..)

add_executable(PID
        analysis.cpp
        analysis.h
        main.cpp
        tune.cpp
        pid/pid.c
        pid/pid.h)
