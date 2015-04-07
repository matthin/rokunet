cmake_minimum_required(VERSION 2.8)
project(rokunet CXX)

add_subdirectory(Source)
add_subdirectory(Source/Http)
include_directories(Source)
include_directories(Source/Http)

add_library(
    rokunet
    STATIC
    ${SourceFiles}
)

ADD_DEFINITIONS(-std=c++11)
SET(CMAKE_EXE_LINKER_FLAGS "-pthread")

add_subdirectory(Tests)
enable_testing()
add_test(NAME Test COMMAND Test)

