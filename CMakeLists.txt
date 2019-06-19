cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 17)

project(ICCAD_2017_B)
include_directories(include)
add_subdirectory("ext-libs/rapidcheck")
add_subdirectory("lib")
add_subdirectory("test")
add_subdirectory("app")
# subdirs(app)

find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)


# set(DOXYGEN_GENERATE_HTML YES)
# set(DOXYGEN_GENERATE_MAN YES)
#
# doxygen_add_docs(
#   doxygen
#    ${PROJECT_SOURCE_DIR}/include
#    ${PROJECT_SOURCE_DIR}/lib
#    ${PROJECT_SOURCE_DIR}/test
#    COMMENT "Generate Doxygen docs"
#)