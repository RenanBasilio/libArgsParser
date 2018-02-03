/**
 * autohelp.cpp
 * 
 * This file contains the implementation for auto-help methods included in the
 * Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
    bool Parser::enable_autohelp(){
        // First test if both switches and the name are available.
        if (registered_symbols.count("Help") == 0 && 
            registered_symbols.count("h") == 0 && 
            registered_symbols.count("help") == 0)
        {
            // To-Do: Register help container.
            return true;
        }
        else return false;
    }
}