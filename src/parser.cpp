/**
 * parser.cpp
 * 
 * This file contains the implementation for general purpose methods of the
 * Parser class. This includes it's constructors, destructors and supporting
 * structures.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parserImpl.h>

namespace ArgsParser
{
    Parser::Parser():
        parser_impl(new ParserImpl()),
        error_description(parser_impl->error_description)
        { }

    Parser::Parser(bool autohelp, bool validation_always_critical):
        parser_impl(new ParserImpl()),
        error_description(parser_impl->error_description)
    {
        if(autohelp) enable_autohelp();
        if(validation_always_critical) parser_impl->validation_always_critical = true;
    }

    Parser::~Parser(){
        delete parser_impl;
        parser_impl = nullptr;
    }

    bool Parser::isRegistered(std::string identifier){
        return (parser_impl->registered_symbols.count(identifier) == 0 ? false : true);
    }

    bool Parser::enable_autohelp(){
        // First test if both switches and the name are available.
        // As there is a distinct possibility of this method being the first
        // to register anything, we can speed up this test by checking if
        // nothing has been registered yet.
        if (    parser_impl->names.size() == 0 ||
                (   parser_impl->registered_symbols.count("Help") == 0 && 
            parser_impl->registered_symbols.count("h") == 0 && 
            parser_impl->registered_symbols.count("help") == 0)
        )
        {
            // To-Do: Register help container.
            return true;
        }
        else return false;
    }
}