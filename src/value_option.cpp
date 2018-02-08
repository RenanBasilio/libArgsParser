/**
 * parameter_option.cpp
 * 
 * This file contains the implementation of the methods for registering
 * and handling value options.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/containerImpl.h>
#include <argsparser/parserimpl.h>

namespace ArgsParser
{
    bool Parser::register_value_option(
        std::string name,
        std::vector<std::string> identifiers,
        std::string placeholder_text,
        std::string description,
        unsigned int min_values,
        unsigned int max_values,
        Validator validator,
        Callback callback
    ) {
        try{
            // First check if all identifiers are open to be registered.
            if(isRegistered(name)) 
                throw std::runtime_error( "Name \"" + name + "\" is already registered." );

            for(size_t i = 0; i < identifiers.size(); i++)
            {
                identifiers[i] = parser_impl->make_identifier(identifiers[i]);
                if(isRegistered(identifiers[i]))
                    throw std::runtime_error( "Identifier \"" + identifiers[i] + "\" is already registered." );
            }
        
            // If check was successful, create a new container object.

            Container* item = parser_impl->make_container(
                name,
                ArgType::Value,
                min_values,
                max_values,
                identifiers, 
                placeholder_text, 
                description, 
                parser_impl->validation_always_critical,
                validator,
                nullptr);

            // And register it with the identifiers provided.

            // Names are preceded by & when registered to differentiate from
            // identifiers with the same name.
            parser_impl->registered_symbols[(name)] = item;
            for (size_t i = 0; i < identifiers.size(); i++)
            {
                // Identifiers are registered directly for faster lookup when
                // parsing.
                parser_impl->registered_symbols[identifiers[i]] = item;
            }

            return true;
        }
        catch (const std::exception& e) {
            if(parser_impl->errors_critical) throw;
            
            parser_impl->error_description = std::string("Registration Error: ") + e.what();
            return false;
        }
        
    };

    bool Parser::register_value_option(
        std::string name,
        std::vector<std::string> identifiers,
        Validator validator,
        Callback callback
    ) {
        return register_value_option(
            name,
            identifiers,
            "value",
            "",
            1,
            1,
            validator,
            callback
        );
    };

    bool Parser::register_value_option(
        std::string name,
        std::vector<std::string> identifiers,
        unsigned int max_values,
        unsigned int min_values,
        Validator validator,
        Callback callback
    ) {
        return register_value_option(
            name,
            identifiers,
            "value",
            "",
            max_values,
            min_values,
            validator,
            callback
        );
    };

    bool Parser::register_value_option(
        std::string name,
        std::vector<std::string> identifiers,
        std::string placeholder_text,
        std::string description,
        Validator validator,
        Callback callback
    ) {
        return register_value_option(
            name,
            identifiers,
            placeholder_text,
            description,
            1,
            1,
            validator,
            callback
        );
    };
}