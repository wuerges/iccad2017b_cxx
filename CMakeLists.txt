cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 14)

project(ICCAD_2017_B)
include_directories(include)
#add_subdirectory("rapidcheck")
add_subdirectory("lib")
add_subdirectory("test")
#add_subdirectory("app")
# subdirs(app)

