# This file contains instructions to build cmake tests.

macro(add_regex_test name args result)
add_test(${name} ${TEST_EXE} ${args})
set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro(add_regex_test name args result)

include(CTest)
set(TEST_EXE ArgsParserTest)
add_executable(${TEST_EXE} test/main.cpp)
target_link_libraries(${TEST_EXE} ${LIB_NAME})

add_test( ParserRuns ${TEST_EXE} "")

add_regex_test( RegistrationSucceeds "" "Registration successful for name \"test\".")
add_regex_test( SecondRegistrationFails "" "Registration Error: Name \"test\" is already registered.")
add_regex_test( InvalidRegistrationFails "" "Registration Error: Name \"!test\" is invalid.")