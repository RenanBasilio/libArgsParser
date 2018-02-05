/**
 * parameter_option.cpp
 * 
 * This file contains the implementation of the methods for registering
 * and handling parameter options to the argument parser.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parserimpl.h>

namespace ArgsParser
{
    bool Parser::register_parameter_option(
        std::string name,
        std::vector<std::string> identifiers,
        std::string placeholder_text,
        std::string description,
        Validator validator,
        Postprocessor postprocessor
    )
    {
        try{
            // First check if all identifiers are open to be registered.
            // To-Do: String sanitization.
            for(size_t i = 0; i < identifiers.size(); i++)
            {
                if(isRegistered(identifiers[i])) throw new std::runtime_error(
                    "Registration Error: Identifier \"" + identifiers[i] + "\" is already registered." );
            }
        
            // If check was successful, create a new container object.
            Container* item = new Container(
                name, 
                identifiers, 
                placeholder_text, 
                description, 
                (false || parser_impl->validation_always_critical),
                validator,
                postprocessor,
                nullptr);

            // And register it with the identifiers provided.
            for (size_t i = 0; i < identifiers.size(); i++)
            {
                parser_impl->registered_symbols[identifiers[i]] = item;
            }

            return true;
        }
        catch (const std::exception& e) {
            parser_impl->error_description = e.what();
            e.~exception();
            return false;
        }
        
    }
}