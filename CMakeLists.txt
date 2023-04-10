cmake_minimum_required(VERSION 3.7.2)
project(natanhp_id)

set(CMAKE_CXX_STANDARD 14)

# Include Conan-generated CMake file
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

SET(WT_CONNECTOR wthttp)
#set(D_GLIBCXX_USE_CXX11_ABI,"0")
#set(CMAKE_CXX_FLAGS "-lboost_program_options")
#set(CXXFLAGS, "-std=c++11")
ADD_SUBDIRECTORY(source)
