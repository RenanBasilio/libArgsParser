#include <iostream>
#include <argsparser.h>

int main(){
    // Initialization Test
    ArgsParser::Parser testParser = ArgsParser::Parser();
    testParser.enable_autohelp();

    // Value Registration Test
    testParser.register_value_option("test", {"-t", "--test"}, 1, 1, "", "A test option.", nullptr, nullptr);
    std::cout << "REGTEST " << (testParser.isRegistered("test") ? "Registration successful for name \"test\"." : testParser.error_description) << std::endl;
    
    // Duplicate Value Registration Test
    testParser.register_value_option("test", {"tt", "testfail"}, 1, 1, "", "A duplicate test option.", nullptr, nullptr);
    std::cout << "DUPREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.register_value_option("duptest", {"--t", "-test"}, 1, 1, "", "A duplicate test option.", nullptr, nullptr);
    std::cout << "DUPREG_TEST_2 " << testParser.error_description << std::endl;

    // Invalid Value Registration Test
    testParser.register_value_option("invalid test", {"!t", "invalid-test"}, 1, 1, "", "A registration failure test.", nullptr, nullptr);
    std::cout << "INVREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.register_value_option("invalid test 2", {"tt", "invalid-test-"}, 1, 1, "", "A registration failure test.", nullptr, nullptr);
    std::cout << "INVREG_TEST_2 " << testParser.error_description << std::endl;
    return 0;
}