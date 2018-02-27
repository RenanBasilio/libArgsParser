#include <iostream>
#include <argsparser.h>
#include <unordered_map>

void print_string(std::string& string){
    std::cout << string << std::endl;
}

int main(){
    // Initialization Test
    ArgsParser::Parser testParser = ArgsParser::Parser();
    testParser.enable_autohelp();

    // Value Registration Test
    testParser.register_option("test", {"-t", "--test"}, "", "A test option.", 1, 1, nullptr, nullptr);
    std::cout << "REG_TEST " << (testParser.isRegistered("test") ? "Registration successful for name \"test\"." : testParser.error_description) << std::endl;
    testParser.register_option<std::string>("nmid", {"-name", "--id"});
    std::cout << "QREG_TEST " << (testParser.isRegistered("nmid") ? "Registration successful for name \"nmid\"." : testParser.error_description) << std::endl;

    // Duplicate Value Registration Test
    testParser.register_option("test", {"tt", "testfail"}, "", "A duplicate test option.", 1, 1, nullptr, nullptr, nullptr);
    std::cout << "DUPREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.register_option<std::string>("duptest", {"--t", "-test"}, "", "A duplicate test option.");
    std::cout << "DUPREG_TEST_2 " << testParser.error_description << std::endl;

    // Invalid Value Registration Test
    testParser.register_option("invalid test", {"!t", "invalid-test"}, "", "A registration failure test.");
    std::cout << "INVREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.register_option<std::string>("invalid test 2", {"tt", "invalid-test-"},  "", "A registration failure test.", 1, 1, nullptr, nullptr);
    std::cout << "INVREG_TEST_2 " << testParser.error_description << std::endl;

    // Lambda Expression Registration Test
    testParser.register_switch(
        "lambda_test", 
        {"l", "lambda"}, 
        "Test registration of a lambda expression callback with additional parameters.",
        [](){print_string(std::string("LAMBDA_TEST_SWITCH"));}
    );
    std::string success_message = "Successful conversion to int.";
    testParser.register_option<int>(
        "lambda_test_2",
        {"lambdab"},
        "value",
        "Test registration of a lambda expression converter with additional parameters.",
        1,
        1,
        [](std::string input){if(input.size() > 5 || std::stoi(input)) return false; else return true; },
        [](std::string input)->int{return std::stoi(input);},
        [testParser](){std::cout << "Conversion to int failed: " << testParser.error_description << std::endl;},
        [&success_message](){print_string(success_message);}
    );
    std::cout << "Debug";

    return 0;
}