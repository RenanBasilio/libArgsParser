# ArgsParser - C++ Command Line Argument Parser

ArgsParser is a C++ static library written with the goal of providing a simple yet complete command line argument parser for C++.

This project is intended to use only STL and Standard Library provided methods in order to create a library which can be easily included in any project, no matter how big or small, without the need for extra dependencies or large libraries of unnecessary methods.

As a library with no dependencies other than the C++ Standard Library, all code in this repository can be merely copied and pasted into any project. This is supported by the MIT License; the code may be used or modified free of charge so long as credit is given to the original author(s).

The below instructions apply to anyone intending to use this library as a static linked library and building it from source.

### Table of Contents

- [Quick Start](#quick-start)
    - [Requirements](#requirements)
    - [Building From Source](#building-from-source)
- [Features](#features)
- [Suggestions and Bug Reports](#suggestions-and-bug-reports)

# Quick Start
## Requirements
1. [CMake](https://cmake.org/) should be installed and in the system `PATH`.
2. A C++ compiler for your operating system. Below are a few recommendations.
    * Windows: [MSBuild](http://landinghub.visualstudio.com/visual-cpp-build-tools)
    * MAC: [clang](https://clang.llvm.org/)
    * Linux: [gcc/g++](https://gcc.gnu.org/)

## Building From Source
1. Create a `build` directory and move into it: `mkdir build && cd build`
2. Use CMake to generate build scripts: `cmake ..`
3. Build the static library: `cmake --build . --config Release`

# Features

In order to provide an easy yet high quality command line parser, the following features are provided:

While the following are planned:
* Registering and retrieving named options.
* Registering and retrieving named parameters.
* Registering and retrieving switches.
* Validation of input parameters.
* Calling a function when an option is passed.
* Automatically generating `help` usage text.

# Suggestions and Bug Reports

If you happen to encounter any bugs while using this library, or have any suggestion of additional features you would like to see implemented, please open an [issue on GitHub](https://github.com/RenanBasilio/ArgsParser/issues).