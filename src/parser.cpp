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

    Parser::Parser(bool autohelp, bool errors_critical, bool validation_critical):
        parser_impl(new ParserImpl()),
        error_description(parser_impl->error_description)
    {
        if(autohelp) enable_autohelp();
        if(validation_critical) parser_impl->validation_always_critical = true;
        if(errors_critical) parser_impl->errors_critical = true;
    }

    Parser::Parser(const Parser &parser):
        parser_impl(new ParserImpl()),
        error_description(parser_impl->error_description)
    {
        if(parser.parser_impl->autohelp_enabled) enable_autohelp();
        if(parser.parser_impl->validation_always_critical) 
            parser_impl->validation_always_critical = true;
        if(parser.parser_impl->errors_critical) parser_impl->errors_critical = true;
        parser_impl->error_description = parser.parser_impl->error_description;
        parser_impl->current = parser.parser_impl->current;
        parser_impl->program_name = parser.parser_impl->program_name;
        parser_impl->registered_symbols = parser.parser_impl->registered_symbols;

    }

    Parser& Parser::operator=(Parser other){
        // Check for self-assignment.
        if (&other == this) return *this;
        
        // Reuse the implementation so both interfaces share the same internals.
        parser_impl = other.parser_impl;
        return *this;
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
        for (it = registered_symbols.begin(); it != registered_symbols.end(); ++it){
            if(it->first.at(0) == '&') {
                // Check if this key is its name.
                const std::string name = it->second->getName();
                if( name == it->first.c_str()){
                    // Get list of registered identifiers from the container.
                    const std::vector<std::string> identifiers = it->second->getIdentifiers();

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
        const char* name,
        ArgType type,
        size_t min_values,
        size_t max_values,
        const std::vector<std::string> &identifiers,
        const char* placeholder_text,
        const char* description,
        bool validation_critical,
        Validator validator,
        Callback error_callback,
        Callback callback
    )
    {
        Container::ContainerImpl* backend = new Container::ContainerImpl(
            name,
            type,
            min_values,
            max_values,
            identifiers,
            placeholder_text,
            description, 
            validation_critical,
            validator,
            error_callback,
            callback);

        Container* interface = new Container(backend);

        return interface;
    };

    std::string Parser::ParserImpl::make_identifier(std::string string){

        // First check if this is a valid string.
        const size_t invalid_char = string.find_first_not_of("-AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
        if(invalid_char != std::string::npos) {
            throw std::runtime_error( std::string("Invalid character '") + string.at(invalid_char) + std::string("'") );
        }
        // Then check if the string contains at least one letter (and does not end in a dash)
        if(string.size() > 0 && string.at(string.size()-1) == '-'){
            throw std::runtime_error( "Identifier must not end in a dash.");
        }

        // Next, check if string is correctly prefixed. If not, add prefix dashes.
        // If string is 1 character long, prefix a dash to it.
        if (string.size() == 1) string.insert(string.begin(), '-');
        // Otherwise, get how many prefix '-' characters it has and add enough to make two.
        else {
            const size_t first_char = string.find_first_not_of("-");

            // If first character is the third character in the string, return.
            // Otherwise, fix the prefix.
            if( (string.size() != 2 && string.at(0) != '-') || first_char != 2)
            {
                std::string identifier = string.substr(first_char);
                string = identifier.size() == 1? ("-" + identifier) : ("--" + identifier);
            }
        }

        return string;
    };
}
