# CMake build script for ArgsParser Library
#
# Building from source:
# > mkdir build && cd build
# > cmake ..
# > cmake --build .
# Note, to build release configuration on Windows with Visual Studio, use instead
# > cmake --build . --config Release
#
# Testing:
# > ctest -V
#
# Install:
# > cmake --build . --target install

cmake_minimum_required(VERSION 3.9)
project(argsparser DESCRIPTION "A simple C++ command line argument parser.")
include(GNUInstallDirs)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 98)

# Instructions to build static library
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(
    SOURCE_FILES 
    src/autohelp.cpp
    src/container.cpp
    src/parser.cpp
    src/positional_option.cpp
    src/value_option.cpp
)

set(
    HEADER_FILES 
    include/argsparser.h
    include/argsparser/autohelp.h
    include/argsparser/common.h
    include/argsparser/container.h
    include/argsparser/parser.h
)

set(
    PRIVATE_HEADER_FILES
    include/argsparser/containerImpl.h
    include/argsparser/parserImpl.h
)

set(LIB_NAME libargsparser)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
set_target_properties(
    ${LIB_NAME} PROPERTIES
    VERSION 1
    SOVERSION 1
    PUBLIC_HEADER "${HEADER_FILES}"
    PRIVATE_HEADER "${PRIVATE_HEADER_FILES}")

target_include_directories(${LIB_NAME} PUBLIC include)
target_include_directories(${LIB_NAME} PUBLIC include/argsparser)

configure_file(ArgsParser.pc.in ArgsParser.pc @ONLY)
install(TARGETS ${LIB_NAME} 
    LIBRARY DESTINATION lib 
    PUBLIC_HEADER DESTINATION include/${LIB_NAME}
    ARCHIVE DESTINATION include/${LIB_NAME}/static)

# Instructions to build test executable
option(BUILD_ARGSPARSER_TESTS "Build Tests." ON)
if(BUILD_ARGSPARSER_TESTS)
    include(test/CMakeTests.cmake)
endif(BUILD_ARGSPARSER_TESTS)

# Instructions to build demo executable
option(BUILD_ARGSPARSER_DEMO "Build Functionality Demo." ON)
if(BUILD_ARGSPARSER_DEMO)
    include(demo/CMakeDemo.cmake)
endif(BUILD_ARGSPARSER_DEMO)