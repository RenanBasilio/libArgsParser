# This file contains instructions to build cmake tests.

macro(add_regex_test name regex)
    # Cannot use ARGN directly with list() command,
    # so copy it to a variable first.
    set (test_parameters ${ARGN})

    add_test(NAME ${name} COMMAND ${TEST_EXE} ${test_parameters})
    set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${regex})
endmacro(add_regex_test name regex)

include(CTest)
set(TEST_EXE ArgsParserTest)
add_executable(${TEST_EXE} test/main.cpp)
target_link_libraries(${TEST_EXE} ${LIB_NAME})

add_test( ParserRuns ${TEST_EXE} )

add_regex_test( RegistrationSucceeds "REG_TEST Registration successful for name \"test\".")
add_regex_test( QuickRegistrationSucceeds "QREG_TEST Registration successful for name \"nmid\".")
add_regex_test( DuplicateRegistrationFails "DUPREG_TEST_1 Registration Error: Name \"test\" is already registered.")
add_regex_test( DuplicateRegistrationFails2 "DUPREG_TEST_2 Registration Error: Identifier \"-t\" is already registered.")
add_regex_test( InvalidRegistrationFails "INVREG_TEST_1 Registration Error: Invalid character \'!\'" "")
add_regex_test( InvalidRegistrationFails2 "INVREG_TEST_2 Registration Error: Identifier must not end in a dash.")
add_regex_test( BasicWorkflowTestINT "BASIC_WORKFLOW_TEST_INT: Returned value 1234567890i." --bwti 1234567890)