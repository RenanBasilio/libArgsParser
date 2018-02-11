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
        const std::string &name,
        std::vector<std::string> identifiers,
        const std::string &placeholder_text,
        const std::string &description,
        const unsigned int max_values,
        const unsigned int min_values,
        const Validator validator,
        const Callback error_callback,
        const Callback callback
    ) {
        try{
            // First check if all identifiers are open to be registered.
            if(isRegistered(name)) 
                throw std::runtime_error( "Name \"" + std::string(name) + "\" is already registered." );

            for(size_t i = 0; i < identifiers.size(); i++)
            {
                identifiers[i] = parser_impl->make_identifier(identifiers[i]);
                if(isRegistered(identifiers[i]))
                    throw std::runtime_error( "Identifier \"" + std::string(identifiers[i]) + "\" is already registered." );
            }
        
            // If check was successful, create a new container object.

            Container* item = parser_impl->make_container(
                name.c_str(),
                ArgType::Value,
                min_values,
                max_values,
                identifiers,
                placeholder_text.c_str(), 
                description.c_str(),
                validator,
                error_callback,
                callback);

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
            parser_impl->error_description = std::string("Registration Error: ") + e.what();
            return false;
        }
        
    };

    bool Parser::register_value_option(
        const std::string &name,
        const std::vector<std::string> &identifiers,
        Validator validator,
        Callback error_callback,
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
            error_callback,
            callback
        );
    };

    bool Parser::register_value_option(
        const std::string &name,
        const std::vector<std::string> &identifiers,
        unsigned int max_values,
        unsigned int min_values,
        Validator validator,
        Callback error_callback,
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
            error_callback,
            callback
        );
    };

    bool Parser::register_value_option(
        const std::string &name,
        const std::vector<std::string> &identifiers,
        const std::string &placeholder_text,
        const std::string &description,
        Validator validator,
        Callback error_callback,
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
            error_callback,
            callback
        );
    };
}