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

    struct Parser::DataContainer{
        const std::string name;
        const char short_id;
        const std::string long_id;
        const std::string placeholder;
        const std::string desc;
        const bool validation_critical;
        bool (*validator) (const void *);
        void (*callback) (const void *);
        
        std::string value;
        
        // If validation succeeds, this is set to true and the failure reason is left empty.
        // If validation is critical and fails, exception is thrown and not handled.
        // If validation is not critical and fails, exception is thrown and handled, with message recorded in the failure reason.
        bool validation_succeeded;
        std::string validation_failure_reason;

        DataContainer(std::string name, 
            char short_id, 
            std::string long_id, 
            std::string placeholder, 
            std::string desc,
            bool (*validator) (const void *),
            bool validation_critical,
            void (*callback) (const void *)
        ) : name(name),
            short_id(short_id),
            long_id(long_id), 
            placeholder(placeholder),
            desc(desc),
            validator(validator),
            validation_critical(validation_critical),
            callback(callback)
        {

        }
    };
}