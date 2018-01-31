# CMake build script for ArgsParser Library
#
# Building from source:
# > mkdir build && cd build
# > cmake ..
# > cmake --build .
#
# Testing:
# > ctest -V
#
# Install:
# > cmake --build . --target install

CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(ArgsParser VERSION 0.0.1 DESCRIPTION "A simple C++ command line argument parser.")
include(GNUInstallDirs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Instructions to build static library
add_library(ArgsParser STATIC src/argsParser.cpp)
set_target_properties(ArgsParser PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    PUBLIC_HEADER include/argsParser.h)

target_include_directories(ArgsParser PRIVATE src)

configure_file(ArgsParser.pc.in ArgsParser.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/ArgsParse.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Instructions to build test executable
set(APP_EXE ArgsParserTest)
add_executable(${APP_EXE} test/main.cpp)
target_link_libraries(${APP_EXE} ArgsParser)