#include <iostream>
#include <string>
#include <functional>
#include <argsparser.h>

void helloWorld() { std::cout << "Hello World!" << std::endl; };

int main(int argc, char* argv[]){
    
    // Initialize the parser.
    ArgsParser::Parser myParser;

    // Enable autohelp to register -h and --help identifiers.
    myParser.enableAutohelp();

    // Register a switch with a callback function.
    myParser.registerSwitch("switch", {"-s", "--switch"}, "Prints a 'Hello World!' when called.", helloWorld);

    // Register a positional that takes an integer and checks if it is less than a value using lambda expressions.
    int value = 1000;
    myParser.registerPositional<int>(
        "positional",
        "integer",
        [&myParser](){ std::cout << "Entered value " << std::to_string((int)myParser.getValue<int>("positional")) << std::endl; },
        [](const std::string& str)->int{ return std::stoi(str); },
        [value](const int& i)->bool{ bool valid = i < value ? true : false; return valid; },
        [](const std::exception& e){ std::cout << e.what(); }
    );

    // Register an option that takes a string and prints it to the console using a callback method.
    myParser.registerOption(
        "reflect",
        {"-r", "--reflect"},
        "string",
        "Print <string> to the console.",
        1,
        [&myParser](){ std::cout << myParser.getValue("reflect") << std::endl; }
    );
    
    // Parse the command line.
    myParser.parse(argc, argv);

    return 0;
}