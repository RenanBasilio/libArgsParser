/**
 * parser.cpp
 * 
 * This file contains the implementation for general purpose methods of the
 * Parser class. This includes it's constructors, destructors and supporting
 * structures.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
    Parser::Parser(bool autohelp = false) : 
        error_description_(""),
        error_description(error_description_)
    {
        if(autohelp) enable_autohelp();
    }

    Parser::~Parser(){

    }
}