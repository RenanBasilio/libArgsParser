/**
 * positional_option.cpp
 * 
 * This file contains the implementation for methods that register and parse
 * positional options for the Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parserImpl.h>

namespace ArgsParser
{
    bool Parser::register_positional(
        const std::string &name, 
        const std::string &placeholder_text,
        const Validator validator,
        const Callback error_callback,
        const Callback callback)
        {
            try{
                // First check if name is already registered.
                if(isRegistered(name))
                    throw std::runtime_error("Name \"" + std::string(name) + "\" is already registered.");
                
                Container* item = parser_impl->make_container(
                    name.c_str(),
                    ArgType::Positional,
                    1,
                    1,
                    std::vector<std::string>(),
                    placeholder_text.c_str(),
                    "",
                    validator,
                    error_callback,
                    callback
                );

                parser_impl->registered_symbols[name] = item;
                parser_impl->registered_symbols[std::to_string(parser_impl->positionals_registered)] = item;

                return true;
            }
            catch (const std::exception& e){
                parser_impl->error_description = std::string("Registration Error: ") + e.what();
                return false;
            }
            
        };

    bool Parser::register_positional(
        const std::string &name,
        const Validator validator,
        const Callback error_callback,
        const Callback callback
    ){
        return false;
    };
}