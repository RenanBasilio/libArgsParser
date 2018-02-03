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
#include <string>

namespace ArgsParser
{ 
    /**
     * This is the base Parser class. It provides basic parsing and auto-help
     * without additional features such as parameter validation.
     */
    class Parser{
        public:
            Parser(bool autohelp);
            ~Parser();

            const std::string& error_description;

            /**
             * This method registers an input parameter option to the parser. Usage example:
             *      ArgsParser.register_option("File", 'f', "file", "file", 'The file to open');
             * 
             * The above line will enable parsing the following lines:
             *      myprogram -f example.txt
             *      myprogram --file example.txt
             * 
             * And generate the following help text:
             *      myprogram -h
             *          ...
             *          -f, --file <file>      The file to open.
             * 
             * The method returns true if the option was registered successfully. Otherwise
             * it returns false and sets the "error_description" variable to a description of the
             * error.
             * 
             * @param {std::string} name The name of the option.
             * @param {char} short_identifier The single character identifier for the option.
             * @param {std::string} long_identifier The multi character identifier for the option.
             * @param {std::string} placeholder_text The text that will be displayed within <> in the help text.
             * @param {std::string} description The description of the option that will be displayed in the help text.
             * @param {pFunc} validator A function to check if the parameter provided with the option is valid.
             * @param {pFunc} callback A function to be called if the parameter option is passed.
             * @return {bool} Whether the registration of the parameter succeeded.
             */
            bool register_parameter_option(
                std::string name,
                char short_identifier,
                std::string placeholder_text = NULL,
                std::string description = NULL
            );
            bool register_parameter_option(
                std::string name,
                std::string long_identifier,
                std::string placeholder_text = NULL,
                std::string description = NULL
            );
            bool register_parameter_option(
                std::string name, 
                char short_identifier,
                std::string long_identifier, 
                std::string placeholder_text = NULL,
                std::string description = NULL
            );

            /**
             * Calling this method will register the 'h' and 'help' switches.
             * When auto-help is enabled, using either switch will produce the
             * following output in the standard output stream (cout):
             * 
             *      usage: ${program_name} [<parameter1>] ... [<options>]
             *          -s, --switch                        ${switch_description}
             *          ...
             * 
             *      options
             *          -o, --option                        ${option_description}
             *          -p, --parameter-opt <placeholder>   ${parameter_option_description}
             *          ...
             * 
             * This method will return true if the switches registered successfully. Otherwise,
             * if one or both have already been registered, it will return false.
             */
            bool enable_autohelp();

        private:
            /**
             * This internal string stores the description of the last non-critical error.
             */
            std::string error_description_;

            /**
             * This internal boolean stores whether auto-help has been enabled.
             */
            bool autohelp_enabled;

            /**
             * This internal structure stores all information related to a registered argument.
             */
            struct DataContainer;

            /** 
             * This vector stores the names of all declared argument types for generating help text.
             */
            std::vector<std::string> names;

            /** 
             * This map stores key value pairs for quick retrieval of arguments by their identifier or name through
             *      get('Help')
             *      get('h')
             *      get('help')
             * Upon registration of an argument type, both it's identifiers and name will be registered as keys pointing
             * to the DataContainer which contains the remainder of the data.
             */
            std::unordered_map<std::string, DataContainer*> registered_symbols;
    };
}