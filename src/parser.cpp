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
#include <argsparser/containerImpl.h>

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
        try{
            // First test if both switches and the name are available.
            // As there is a distinct possibility of this method being the first
            // to register anything, we can speed up this test by checking if
            // nothing has been registered yet.
            if (parser_impl->registered_symbols.size() == 0 || ( !isRegistered("h") && !isRegistered("help") ))
            {
                // To-Do: Register help container.
                return true;
            }
            else throw new std::runtime_error("Autohelp failure: A keyword is already registered.");
        }
        catch (const std::exception& e){
            parser_impl->error_description = e.what();
            e.~exception();
            return false;
        }
    }

   Parser::ParserImpl::ParserImpl() :
        error_description(""),
        autohelp_enabled(false),
        validation_always_critical(false)
        { };

    Parser::ParserImpl::~ParserImpl(){
        // Iterate through the map, destroying any objects the first time
        // they are found. This produces some processing overhead, but
        // considering it's the final deletion operation for a class that
        // gets instantiated at most once it is not as troublesome as the
        // memory overhead of keeping a reference to each name.

        std::unordered_map<std::string, Container*>::iterator it;
        for (it = registered_symbols.begin(); it != registered_symbols.end(); it++){
            if(it->first.at(0) == '&') {
                // Check if this key is its name.
                const char* name = it->second->getName();
                if( name == it->first.c_str()){
                    // Get list of registered identifiers from the container.
                    std::vector<std::string> identifiers = it->second->getIdentifiers();

                    // For each identifier, if it's not the same as the name, set
                    // it to a nullptr. This guarantees we wont be running into any
                    // dangling references through the rest of the deletion process.
                    for(size_t i = 0; i < identifiers.size(); i++){
                        if(identifiers[i] != name)
                            registered_symbols[identifiers[i]] = nullptr;
                    };

                    // Finally, delete the last reference to it (the name).
                    delete registered_symbols[name];
                    registered_symbols[name] = nullptr;
                }
            }
            else it->second = nullptr;
        }
    };

    Container* Parser::ParserImpl::make_container(
        std::string name,
        ArgType type,
        std::vector<std::string> identifiers,
        std::string placeholder_text,
        std::string description,
        bool validation_critical,
        Validator validator,
        Postprocessor postprocessor,
        Callback callback
    )
    {
        Container::ContainerImpl* backend = new Container::ContainerImpl(
            name,
            type,
            identifiers, 
            placeholder_text, 
            description, 
            validation_critical,
            validator,
            postprocessor,
            callback);

        Container* interface = new Container(backend);

        return interface;
    };
}
