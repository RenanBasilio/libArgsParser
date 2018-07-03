# This file contains instructions to build the demo application using CMake.

set(DEMO_EXE ArgsParserDemo)
add_executable(${DEMO_EXE} demo/main.cpp)
target_link_libraries(${DEMO_EXE} ${LIB_NAME})