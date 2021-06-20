cmake_minimum_required(VERSION 3.15)
project(clusterization)

set(CMAKE_CXX_STANDARD 17)

add_executable(clusterization cluster.h clusterization.cpp)