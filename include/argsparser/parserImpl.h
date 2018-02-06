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
        // This string stores the description of the last non-critical error.
        std::string error_description;

        // This internal boolean stores whether auto-help has been enabled.
        bool autohelp_enabled;

        // This internal boolean stores whether validation failures should throw unhandled exceptions by default.
        bool validation_always_critical;

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

        Container* make_container(
            std::string name,
            ArgType type,
            std::vector<std::string> identifiers,
            std::string placeholder_text,
            std::string description,
            bool validation_critical,
            Validator validator,
            Postprocessor postprocessor,
            Callback callback
        );
    };
}