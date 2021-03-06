#include <iostream>
#include <argsparser.h>
#include <exception>

void print_string(std::string string){
    std::cout << string << std::endl;
}

int main(int argc, char* argv[]){
    // Initialization Test
    ArgsParser::Parser testParser = ArgsParser::Parser();
    std::vector<std::string> argvector(argv+1, argv+argc);
    testParser.enableAutohelp();

    // Value Registration Test
    testParser.registerOption("test", {"-t", "--test"}, "value", "A test option.", 2, nullptr, nullptr);
    std::cout << "REG_TEST " << (testParser.isRegistered("test") ? "Registration successful for name \"test\"." : testParser.error_description) << std::endl;
    testParser.registerOption<std::string>("nmid", {"-name", "--id"});
    std::cout << "QREG_TEST " << (testParser.isRegistered("nmid") ? "Registration successful for name \"nmid\"." : testParser.error_description) << std::endl;

    // Duplicate Value Registration Test
    testParser.registerOption("test", {"tt", "testfail"}, "value", "A duplicate test option.", 1, nullptr, nullptr, nullptr);
    std::cout << "DUPREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.registerOption<std::string>("duptest", {"--t", "-test"}, "value", "A duplicate test option.");
    std::cout << "DUPREG_TEST_2 " << testParser.error_description << std::endl;

    // Invalid Value Registration Test
    testParser.registerOption("invalid test", {"!t", "invalid-test"}, "value", "A registration failure test.");
    std::cout << "INVREG_TEST_1 " << testParser.error_description << std::endl;
    testParser.registerOption<std::string>("invalid test 2", {"tt", "invalid-test-"},  "value", "A registration failure test.", 1, nullptr, nullptr);
    std::cout << "INVREG_TEST_2 " << testParser.error_description << std::endl;

    // Lambda Expression Registration Test
    testParser.registerSwitch(
        "lambda_test", 
        {"l", "lambda"},
        "Test registration of a lambda expression callback with additional parameters.",
        [](){print_string(std::string("LAMBDA_TEST_SWITCH"));}
    );
    std::string success_message = "Successful conversion to int.";
    testParser.registerOption<int>(
        "lambda_test_2",
        {"lambdab"},
        "value",
        "Test registration of a lambda expression converter with additional parameters.",
        1,
        [&success_message](){print_string(success_message);},
        [](std::string input)->int{return std::stoi(input);},
        [](int input){if(input > 2000) return false; else return true; },
        [testParser](const std::exception& e){std::cout << "Conversion to int failed: " << e.what() << std::endl;}
    );
    std::cout << "LAMBDA_REG_TEST " << testParser.error_description << std::endl;

    ArgsParser::Token bwti;
    bwti = testParser.registerOption<int>(
        "basic workflow test <int>",
        {"bwti"},
        "integer",
        "Test basic workflow including retrieval of an integer option.",
        1,
        [testParser, success_message, bwti](){std::cout << success_message << std::endl;},
        [](std::string input)->int{return std::stoi(input);},
        [](int input){return true;},
        [testParser](const std::exception& e){ArgsTools::print_error(e);}
    );
    std::cout << "LAMBDA_REG_TEST2 " << testParser.error_description << std::endl;
    testParser.parse(argc, argv);
    std::cout << "BASIC_WORKFLOW_TEST_INT: " << "Returned value " << std::to_string((int)(testParser.getValue<int>("bwti"))) << "i." << std::endl;

    std::cout << "Debug";

    return 0;
}