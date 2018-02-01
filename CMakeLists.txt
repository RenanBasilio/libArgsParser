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

CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(ArgsParser VERSION 0.0.1 DESCRIPTION "A simple C++ command line argument parser.")
include(GNUInstallDirs)
enable_language(CXX)
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
option(BUILD_ARGSPARSER_TESTS "Build tests." ON)
if(BUILD_ARGSPARSER_TESTS)
    set(TEST_EXE ArgsParserTest)
    add_executable(${TEST_EXE} test/main.cpp)
    target_link_libraries(${TEST_EXE} ArgsParser)    
endif(BUILD_ARGSPARSER_TESTS)

# Instructions to build demo executable
option(BUILD_ARGSPARSER_DEMO "Build a functionality demo." ON)
if(BUILD_ARGSPARSER_DEMO)
    set(DEMO_EXE ArgsParserDemo)
    add_executable(${DEMO_EXE} demo/main.cpp)
    target_link_libraries(${DEMO_EXE} ArgsParser)
endif(BUILD_ARGSPARSER_DEMO)