/**
 * parameter_option.cpp
 * 
 * This file contains the implementation of the methods for registering
 * and handling parameter options to the argument parser.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
    bool Parser::register_parameter_option(
        std::string name,
        std::string identifiers,
        std::string placeholder_text,
        std::string description,
        Validator validator,
        Postprocessor callback
    )
    {
        return false;
    }
}