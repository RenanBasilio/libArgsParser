#include <iostream>
#include <argsparser.h>

int main(){
    // Initialization Test
    ArgsParser::Parser testParser = ArgsParser::Parser();
    testParser.enable_autohelp();

    // Value Registration Test
    testParser.register_value_option("test", {"-t", "--test"}, "", "A test option.", 1, 1, nullptr, nullptr);
    std::cout << "REG_TEST " << (testParser.isRegistered("test") ? "Registration successful for name \"test\"." : testParser.error_description) << std::endl;
    testParser.register_value_option("nmid", {"-name", "--id"});
    std::cout << "QREG_TEST " << (testParser.isRegistered("nmid") ? "Registration successful for name \"nmid\"." : testParser.error_description) << std::endl;

    // Duplicate Value Registration Test
    testParser.register_value_option("test", {"tt", "testfail"}, "", "A duplicate test option.", 1, 1, nullptr, nullptr);
    std::cout << "DUPREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.register_value_option("duptest", {"--t", "-test"}, "", "A duplicate test option.");
    std::cout << "DUPREG_TEST_2 " << testParser.error_description << std::endl;

    // Invalid Value Registration Test
    testParser.register_value_option("invalid test", {"!t", "invalid-test"}, "", "A registration failure test.");
    std::cout << "INVREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.register_value_option("invalid test 2", {"tt", "invalid-test-"},  "", "A registration failure test.", 1, 1, nullptr, nullptr);
    std::cout << "INVREG_TEST_2 " << testParser.error_description << std::endl;
    return 0;
}