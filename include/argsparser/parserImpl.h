/**
 * parserImpl.h
 * 
 * This file contains the implementation of the internals of the Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
        struct Parser::ParserImpl{
        // This string stores the description of the last non-critical error.
        std::string error_description;

        // This internal boolean stores whether auto-help has been enabled.
        bool autohelp_enabled;

        // This internal boolean stores whether validation failures should throw unhandled exceptions by default.
        bool validation_always_critical;
 
        // This vector stores the names of all declared argument types for generating help text.
        std::vector<std::string> names;

        /** 
         * This map stores key value pairs for quick retrieval of arguments by their identifier or name through
         *      get('Help')
         *      get('h')
         *      get('help')
         * Upon registration of an argument type, both it's identifiers and name will be registered as keys pointing
         * to the DataContainer which contains the remainder of the data.
         */
        std::unordered_map<std::string, Container*> registered_symbols;

        ParserImpl::ParserImpl():
            error_description(""),
            autohelp_enabled(false),
            validation_always_critical(false)
        { };

        ParserImpl::~ParserImpl(){
            // Since all identifiers registered to one name share the same container
            // object we only need to free one pointer per name.
            for (size_t i = 0; i < names.size(); i++){
                delete registered_symbols.at(names[i]);
            }
        }

    };
}