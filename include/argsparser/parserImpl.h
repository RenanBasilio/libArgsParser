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

        /**
         * These maps stores name and symbol information respectively. The name
         * map maps user entered names (such as "help") to the respective type
         * of argument and identifier so it may be quickly accessed from the
         * corresponding vector, while the symbol maps map command line symbols
         * (such as "-h") for access when parsing the command line.
         */
        std::unordered_map<std::string, token> names;
        std::unordered_map<std::string, token> identifiers;

        /**
         * These vectors contain pointers to the actual containers storing each
         * argument type.
         * Positionals are read in the order they are declared, so this vector
         * can be used to retrieve their containers faster than a map lookup.
         */
        std::vector<Container*> registered_positionals;
        std::vector<Container*> registered_switches;
        std::vector<Container*> registered_options;

        ParserImpl();

        ~ParserImpl();

        unsigned int current = 0;

        const char* getNext(){
            // TODO: Implement method to get the next element from the array.
            current++;
        };
    };
}