cmake_minimum_required(VERSION 3.23)

project(io LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(io INTERFACE)
target_compile_features(io INTERFACE cxx_std_17)

set(LIB_TARGETS io PARENT_SCOPE)