#include <iostream>
#include <argsparser.h>

int main(){
    ArgsParser::Parser testParser = ArgsParser::Parser();
    testParser.enable_autohelp();
    testParser.register_value_option("test", {"t", "test"}, 1, 1, "", "A test option.", nullptr, nullptr);
    std::cout << (testParser.isRegistered("test") ? "Registration successful for name \"test\"." : testParser.error_description) << std::endl;
    testParser.register_value_option("test", {"t", "test"}, 1, 1, "", "A test option.", nullptr, nullptr);
    std::cout << testParser.error_description << std::endl;
    testParser.register_value_option("!test", {"f", "fail-test"}, 1, 1, "", "A registration failure test.", nullptr, nullptr);
    return 0;
}