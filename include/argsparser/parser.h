/**
 * parser.h
 * 
 * This file contains the class declaration for the base Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <argsparser/autohelp.h>

namespace ArgsParser
{ 
    /**
     * This is the base Parser class. It provides basic parsing and auto-help
     * without additional features such as parameter validation.
     */
    class Parser{
        private:
            struct ParserImpl;
            ParserImpl* parser_impl;

        public:
            Parser();
            Parser(bool autohelp, bool errors_critical = false, bool validation_critical = false);
            Parser(const Parser &parser);
            Parser& operator=(Parser other);
            
            ~Parser();

            const std::string& error_description;

            bool isRegistered(std::string identifiers);

            /**
             * This method register a positional argument to the parser. Usage example:
             *      ArgsParser.register_positional(
             *          "file", "filename", &file_exists, &show_error, &callback);
             * 
             * The above line will enable parsing the following lines:
             *      myapp example.txt
             * The parser will then call file_exists(example.txt) and if true then callback(),
             * otherwise show_error().
             * 
             * It will generate the following help text:
             *      usage: myapp [<filename>]
             * 
             * Positionals are parsed in the order they are registered.
             * 
             * The method returns true if the option was registered successfully. Otherwise
             * it returns false and sets the "error_description" variable to a description of the
             * error.
             * 
             * @param {std::string} name The name of the positional.
             * @param {std::string} placeholder_text The text to show in a help message to describe the positional.
             * @param {Validator} validator The method to use to validate the user input string.
             * @param {Callback} error_callback A method to call in case validation fails.
             * @param {Callback} callback A method to call in case validation succeeds.
             * @return {bool} Whether the registration of the parameter succeeded.
             */
            bool register_positional(
                std::string name,
                std::string placeholder_text,
                Validator validator = nullptr,
                Callback error_callback = nullptr,
                Callback callback = nullptr
            );

            /**
             * This method registers an input value option to the parser. Usage example:
             *      ArgsParser.register_option(
             *          "file", {"f", "file"}, "filename", "The file to open", &file_exists, &show_error, &callback);
             * 
             * The above line will enable parsing the following lines:
             *      myapp -f example.txt
             *      myapp --file example.txt
             * The parser will then call file_exists(example.txt) and if true then callback(), otherwise
             * then show_error().
             * 
             * And generate the following help text:
             *      -f, --file <filename>      The file to open.
             * 
             * The method returns true if the option was registered successfully. Otherwise
             * it returns false and sets the "error_description" variable to a description of the
             * error.
             * 
             * @param {std::string} name The name of the option.
             * @param {std::vector<std::string>} identifiers The identifiers to register for this option
             * @param {std::string} placeholder_text The text that will be displayed within <> in the help text.
             * @param {std::string} description The description of the option that will be displayed in the help text.
             * @param {Validator} validator A function to check if the parameter provided with the option is valid.
             * @param {Callback} error_callback A function to call if validation fails.
             * @param {Callback} callback A function to be called if validation succeeds.
             * @return {bool} Whether the registration of the parameter succeeded.
             */
            bool register_value_option(
                const std::string &name,
                std::vector<std::string> identifiers,
                const std::string &placeholder_text,
                const std::string &description,
                const unsigned int max_values,
                const unsigned int min_values = 1,
                const Validator validator = nullptr,
                const Callback error_callback = nullptr,
                const Callback callback = nullptr
            );
            bool register_value_option(
                const std::string &name,
                const std::vector<std::string> &identifiers,
                const Validator validator = nullptr,
                const Callback error_callback = nullptr,
                const Callback callback = nullptr
            );
            bool register_value_option(
                const std::string &name,
                const std::vector<std::string> &identifiers,
                const unsigned int max_values,
                const unsigned int min_values = 1,
                const Validator validator = nullptr,
                const Callback error_callback = nullptr,
                const Callback callback = nullptr
            );
            bool register_value_option(
                const std::string &name,
                const std::vector<std::string> &identifiers,
                const std::string &placeholder_text,
                const std::string &description,
                const Validator validator = nullptr,
                const Callback error_callback = nullptr,
                const Callback callback = nullptr
            );

            /**
             * Calling this method will register the 'h' and 'help' switches under 'help'.
             * 
             * This method will return true if the switches registered successfully. Otherwise,
             * if one or both have already been registered, it will return false.
             * 
             * Alternatively, if you wish to register your own help provider, register a
             * switch with the method you wish to use as the callback function.
             */
            bool enable_autohelp();
    };
}