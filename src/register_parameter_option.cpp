/**
 * register_parameter_option.cpp
 * 
 * This file contains the implementation of the methods for registering
 * parameter options to the argument parser.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
    bool Parser::register_parameter_option(
        std::string name,
        std::string short_id,
        std::string long_id,
        std::string placeholder_text,
        std::string description
    )
    {
        // Check if at least one identifier was provided
        if (short_id == "" && long_id == ""){
            // To-Do: Registration failed.
            return false;
        }
        if (registered_symbols.count(name) == 0 && 
            registered_symbols.count(short_id) == 0 && 
            registered_symbols.count("help") == 0) return false; // To-Do: Register
    }
}