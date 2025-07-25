cmake_minimum_required(VERSION 3.23)

project(io LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

#add_library(io INTERFACE)
CREATE_LIB(io INTERFACE)

target_compile_features(io INTERFACE cxx_std_17)

target_include_directories(io INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/io>
)
INSTALL_LIB(io True io)