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
include_directories(".")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEAD_FILES ${PROJECT_SOURCE_DIR}/include/argsparser/*.h)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VERSION 1
    SOVERSION 1
    PUBLIC_HEADER include/argsparser.h ${HEAD_FILES})

target_include_directories(argsparser PRIVATE src)
target_include_directories(argsparser PUBLIC include)
target_include_directories(argsparser PUBLIC include/argsparser)

configure_file(ArgsParser.pc.in ArgsParser.pc @ONLY)
install(TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION lib 
    PUBLIC_HEADER DESTINATION include/argsparser
    ARCHIVE DESTINATION include/argsparser/static)

# Instructions to build test executable
option(BUILD_ARGSPARSER_TESTS "Build tests." ON)
if(BUILD_ARGSPARSER_TESTS)
    set(TEST_EXE ArgsParserTest)
    add_executable(${TEST_EXE} test/main.cpp)
    target_link_libraries(${TEST_EXE} argsparser)    
endif(BUILD_ARGSPARSER_TESTS)

# Instructions to build demo executable
option(BUILD_ARGSPARSER_DEMO "Build a functionality demo." ON)
if(BUILD_ARGSPARSER_DEMO)
    set(DEMO_EXE ArgsParserDemo)
    add_executable(${DEMO_EXE} demo/main.cpp)
    target_link_libraries(${DEMO_EXE} argsparser)
endif(BUILD_ARGSPARSER_DEMO)