/**
 * parserImpl.h
 * 
 * This file contains the declarations of the internals of the Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
    struct Parser::ParserImpl{
        // This string stores the parsed name of the program.
        std::string program_name;

        // This string stores the description of the last non-critical error.
        std::string error_description;

        // This internal boolean stores whether auto-help has been enabled.
        bool autohelp_enabled = false;

        // This internal boolean stores whether parsing errors should be 
        // considered critical and throw unhandled exceptions.
        bool errors_critical = false;

        // This internal boolean stores whether validation failures should
        // throw unhandled exceptions by default.
        bool validation_always_critical = false;

        /** 
         * This map stores key value pairs for quick retrieval of arguments by their identifier or name through
         *      get('Help')
         *      get('h')
         *      get('help')
         * Upon registration of an argument type, both it's identifiers and name will be registered as keys pointing
         * to the DataContainer which contains the remainder of the data.
         */
        std::unordered_map<std::string, Container*> registered_symbols;

        ParserImpl();

        ~ParserImpl();

        /**
         * This method creates a container with the parameters provided.
         * 
         * It is helpful due to the implementation-first logic used to hide the
         * constructor of the container object.
         */
        Container* make_container(
            const char* name,
            ArgType type,
            size_t min_values,
            size_t max_values,
            const std::vector<std::string> &identifiers,
            const char* placeholder_text,
            const char* description,
            bool validation_critical,
            Validator validator,
            Callback error_callback,
            Callback callback
        );

        /**
         * This method makes a valid identifier from a string. this is done by
         * prefixing a single '-' character to the string if it is only one
         * character long and '--' if it contains multiple character.
         * 
         * This method throws an exception if any characters in the string are
         * invalid. Valid characters are:
         *      "-AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
         */
        std::string make_identifier(std::string string);

        unsigned int current = 0;

        const char* getNext(){
            // TODO: Implement method to get the next element from the array.
            current++;
        };
    };
}