# ArgsParser - C++ Command Line Argument Parser
[![Build Status](https://travis-ci.org/RenanBasilio/libArgsParser.svg?branch=master)](https://travis-ci.org/RenanBasilio/libArgsParser)
[![Build status](https://ci.appveyor.com/api/projects/status/y9619y0sipb55wbu/branch/master?svg=true)](https://ci.appveyor.com/project/RenanBasilio/argsparser/branch/master)

ArgsParser is a C++ static library written with the goal of providing an easy to use yet thorough command line argument parser for C++. It is intended to simplify the process of parsing said arguments by providing an interface which can parse, verify and apply any post processing required to user input.

This project was conceived to use only STL and Standard Library provided methods in order to create a library which can be easily included in any project, no matter how big or small, without the need for extra dependencies or large libraries of unnecessary methods.

This project is licensed under the MIT License; the code may be used or modified free of charge so long as credit is given to the original author(s).

The below instructions apply to anyone intending to use this library as a static linked library and building it from source.

### Table of Contents

* [Quick Start](#quick-start)
* [Building From Source](#building-from-source)
    * [Requirements](#requirements)
    * [Step by Step Instructions](#step-by-step-instructions)
* [Features](#features)
* [Suggestions and Bug Reports](#suggestions-and-bug-reports)

# Quick Start


# Building From Source
## Requirements
1. [CMake](https://cmake.org/) should be installed and in the system `PATH`.
2. A C++ compiler for your operating system. Below are a few recommendations.
    * Windows: [MSBuild](http://landinghub.visualstudio.com/visual-cpp-build-tools)
    * MAC: [gcc/g++](http://gcc.gnu.org/) or [clang](https://clang.llvm.org/)
    * Linux: [gcc/g++](https://gcc.gnu.org/)
## Step by Step Instructions
1. Create a `build` directory and move into it: `mkdir build && cd build`
2. Use CMake to generate build scripts: `cmake ..`
3. Build the static library: `cmake --build . `

    If you are using windows and intending to build using the Release configuration, the third step will include the `--config Release` parameter as well:

3. Build the static library: `cmake --build . --config Release`

# Features

This library offers the following functionalities:
* [Command Line Parser](#command-line-parser)
* [Argument Types](#argument-types)
    * [Positionals](#positionals)
    * [Switches](#switches)
    * [Options](#options)
* [Typed Argument Templates](#typed-argument-templates)
* [Post-Processing](#post-processing)
    * [Callbacks](#callbacks)
    * [Converters](#converters)
    * [Validators](#validators)
    * [Error Handlers](#error-handlers)
* [Automatic Usage Text Generation](#automatic-usage-text-generation)

## Command Line Parser

The basic feature of this library, allows for easier usage of command line arguments through the use of a specific workflow when command line arguments should be parsed.

The following code exemplifies this workflow in the context of a "verbose" switch.

    ArgsParser::Parser myParser();          // Instantiate the parser
    
    myParser.registerSwitch(                // Register a switch type argument
        "verbose",                          // The name of the switch
        {"-v", "--verbose"},                // The identifiers of the switch
    );

    myParser.parse(argc, argv);             // Call parse to read the command line input

    if(myParser.getValue["verbose"]) {...}  // Use the result

## Argument Types

The library initially supports three distinct types of command line arguments. These are **_switches_**, **_positionals_** and **_options_**.

### Switches

Switches are arguments which do not take a value. They are used as configuration booleans simply through their presence in the command line. Common examples of switches are `-h` for help text and `-v` for verbose output.

A switch can be registered through the following method:

    ArgsParser::Token registerSwitch(
        const std::string& name,                                    # The name of the switch
        const std::vector<std::string>& identifiers,                # The identifiers to associate with this switch
        const std::string& description = "Description not given.",  # The description to use when generating help text
        const Callback& callback = nullptr                          # A method to call if this switch is found
    );

### Positionals

Positionals are value arguments which are defined by their position in the command line. They often follow the name of the program, and are identified as a single value. 

A positional argument can be registered through the following method:

    ArgsParser::Token registerPositional(
        const std::string& name,                                    # The name of the positional
        const std::string& placeholder_text,                        # The placeholder text to use for usage text generation
        const Callback& callback = nullptr,                         # A method to call if this positional is found
        const Validator<std::string>& validator = nullptr,          # A method to use for validation of the value passed to this positional
        const ErrorHandler& error_callback = nullptr                # A method to call if validation fails
    );

### Options

Options are value arguments which are defined by the identifier which precedes them in the command line. A common example would be `-o filename`, which is used to denote a file in which to place some output.

An option can be registered through the following method:

    ArgsParser::Token registerOption(
        const std::string& name,                                    # The name of the option
        const std::vector<std::string>& identifiers,                # The identifiers to associate with this option
        const std::string& placeholder_text = "value",              # The placeholder text to use when generating usage text
        const std::string& description = "Description not given.",  # The description to use when generating usage text
        const size_t max_values = 1,                                # The maximum number of values that this option can hold
        const Callback& callback = nullptr,                         # A method to call if this option is found
        const Validator<std::string>& validator = nullptr,
        # A method to use for validating the input value(s)
        const ErrorHandler& error_callback = nullptr                # A method to call if validation fails
    );

## Typed Argument Templates

The library provides templates for registering and reading arguments where the type of the value they refer to is not a string. In such cases, the template can be used instead.

In order to allow for inputs of non-standard types, the template method allows the user to specify the method to use for converting the string input to that type (further explained in the [post-processing](#post-processing) section). **As of the current version, default converters are not yet assigned by the registration process. This will be improved upon in future versions.**

The following example shows how to register a positional argument which takes an integer.

    myParser.registerPositional<int>(
        "name",
        "value",
        nullptr,                                                    # This method does not use a callback
        [](const std::string& str)->int{ return std::stoi(str); }   # A converter method that takes a const string reference and returns an integer.
    );

Analogously, the value can be retrieved using the corresponding template of the `getValue` method.

    int myInteger = myParser.getValue<int>("name");

## Post-Processing

The final stage of the parsing process is the post processing of the parsed arguments. This is done using user-provided methods that resemble a given type.

Post processing methods are executed in the order the arguments are passed to the command line. There are plans to implement the ability to defer certain arguments to later passes, in case the value of another argument would be relevant to it's post-processing step.

### Callbacks

Callback methods are methods which take no arguments and return `void`. These methods are called at the end of the post processing step if all checks succeed.

Although they may not take any arguments directly, callback methods with arguments can be achieved through the use of _lambda expressions_. An example of this is the registration of the autohelp method, which uses a reference to the parser that is registering it through the `this` pointer - `[this](){autohelper(this, std::cout);}`.

### Converters

Conversion methods are inherent of template options. These are methods which convert from a string to the type of the template.

In order for a conversion method to be valid, it must take a `const string&` and return an object of the template type.

The following is a valid method which converts a string to an integer, using the default function `stoi`:

    int toInt(const std::string& str) {
        return stoi(str);
    };

**Converter methods are called once for each string in the input list.**

### Validators

Validation methods ensure that a condition can be met by the input value. Such a condition may be, for instance, a file existing.

A validator takes a const object reference of the type of the argument (for arguments registered through the standard methods this is a string; for templates this is the type of the template) and returns a boolean.

Optionally, should more information about the cause of a validation failure be desired, the method may throw an exception. This exception will be caught and passed on to the associated error handler.

This example shows a validator which returns `true` if the value is larger than `1000`, and `false` otherwise.

    bool largerThanOneK(const int& value) {
        if (value > 1000) return true;
        else return false;
    };

**Validator methods are called once for each string or converted value in the input list.**

### Error Handlers

Error handler methods handle an exception. They take a `const std::exception&` and return `void`. **Note that, should a validation failure occur, none of the remaining values for this argument will be validated. As such, it is good practice for this type of method to simply inform the user of this error and exit.**

    void printAndExit(const std::exception& e) {
        std::cout << An error occurred: " << e.what() << std::endl;
        exit(0);
    };

## Automatic Usage Text Generation

The library can automatically provide usage ("help") text through the "-h" and "--help" switches. In order to enable this functionality, call the `enableAutohelp` method on the parser.

    myParser.enableAutohelp();

Help text is generated using the following formatting:

    usage: program_name [positional] ... [args]
        -s, --switch                   A switch.

    options
        -v, --value <placeholder>      An option that takes an input.
        -l, --list [<placeholder>...]  An option that takes a list of inputs

The name of the program is deduced during runtime, and matches the name of the executable from which the code was executed.

# Suggestions and Bug Reports

If you happen to encounter any bugs while using this library, or have any suggestion of additional features you would like to see implemented, please open an [issue on GitHub](https://github.com/RenanBasilio/ArgsParser/issues).
