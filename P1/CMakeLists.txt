cmake_minimum_required(VERSION 3.12)
project(cs410)

set(CMAKE_CXX_STANDARD 14)

include_directories(source)

add_executable(cs410
        source/main.cpp
        source/wavefront_obj.h
        source/driver.h source/model.h source/driver.cpp)
