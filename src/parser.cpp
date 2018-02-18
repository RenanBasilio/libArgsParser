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
        error_description(parser_impl->error_description),
        errors_critical(false)
        { }

    Parser::Parser(bool autohelp, bool errors_critical):
        parser_impl(new ParserImpl()),
        error_description(parser_impl->error_description),
        errors_critical(errors_critical)
    {
        if(autohelp) enable_autohelp();
    }

    Parser::Parser(const Parser& other):
        parser_impl(new ParserImpl(*other.parser_impl)),
        error_description(parser_impl->error_description),
        errors_critical(other.errors_critical)
    { 
    }

    Parser::Parser(Parser&& other):
        Parser()
    {
        swap(*this, other);
    }

    Parser& Parser::operator=(Parser other){

        swap(*this, other);

        return *this;
    }

    Parser::~Parser(){
        delete parser_impl;
        parser_impl = nullptr;
    }

    void swap(Parser& first, Parser& second){
        using std::swap;

        swap(first.parser_impl, second.parser_impl);
    }

    bool Parser::isRegistered(const std::string& symbol){
        return (isNameRegistered(symbol) || isIdentifierRegistered(symbol));
    }

    bool Parser::isNameRegistered(const std::string& name){
        return parser_impl->names.count(name) > 0;
    }

    bool Parser::isIdentifierRegistered(const std::string& identifier){
        // First check if this is a proper identifier. If not, make one.
        const std::string identifier_ = 
            (check_identifier(identifier)? identifier : make_identifier(identifier));

        return parser_impl->identifiers.count(identifier_) > 0;
    }

    bool Parser::enable_autohelp(){
        try{
            // First test if both switches and the name are available.
            // As there is a distinct possibility of this method being the first
            // to register anything, we can speed up this test by checking if
            // nothing has been registered yet.
            if (parser_impl->names.size() == 0 || 
               ( !isNameRegistered("help") && !isIdentifierRegistered("-h") && !isIdentifierRegistered("--help")))
            {
                // To-Do: Register help container.
                return true;
            }
            else throw std::runtime_error("Autohelp failure: A keyword is already registered.");
        }
        catch (const std::exception& e){
            parser_impl->error_description = e.what();
            return false;
        }
    }

    token Parser::register_switch(
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const Callback& callback
    ){
        // First check if all identifiers are open to be registered.
        if(isNameRegistered(name)) 
            throw std::runtime_error( "Name \"" + name + "\" is already registered." );

        std::vector<std::string> identifiers_(identifiers.size());
        for(size_t i = 0; i < identifiers.size(); i++)
        {
            identifiers_[i] = make_identifier(identifiers[i]);
            if(isIdentifierRegistered(identifiers_[i]))
                throw std::runtime_error( "Identifier \"" + identifiers_[i] + "\" is already registered." );
        }

        Container* container = new Container(
            name,
            identifiers_,
            description,
            callback
        );

        token id = register_container(ArgType::Switch, container);

        return id;
    }

    token Parser::register_container(ArgType type, Container* container){
        // The identifier type allows for up to 6553 options of each type to be
        // registered. As such, throw an exception if that amount is reached.

        // Register the container
        size_t index;
        switch (type)
        {
        case ArgType::Positional:
            if (parser_impl->registered_positionals.size() == (std::numeric_limits<token>::max() / 10))
                throw new std::runtime_error("Exceeded maximum number of same type options (>6553).");
            index = parser_impl->registered_positionals.size();
            parser_impl->registered_positionals.push_back(container);
            break;
        case ArgType::Option:
            if (parser_impl->registered_options.size() == (std::numeric_limits<token>::max() / 10))
                throw new std::runtime_error("Exceeded maximum number of same type options (>6553).");
            index = parser_impl->registered_options.size();
            parser_impl->registered_options.push_back(container);
            break;
        case ArgType::Switch:
            if (parser_impl->registered_switches.size() == (std::numeric_limits<token>::max() / 10))
                throw new std::runtime_error("Exceeded maximum number of same type options (>6553).");
            index = parser_impl->registered_switches.size();
            parser_impl->registered_switches.push_back(container);
            break;
        default:
            return null_token;
        }

        // Calculate the id of the container.
        // The last digit of the container is the type (ArgType), while
        // the remaining digits are the index in the container.
        token id = static_cast<token>(( index * 10 ) + ArgType::Positional);
        std::pair<ArgType, token> token_pair = std::make_pair(type, id);

        // Add the id to the map of names.
        parser_impl->names[container->getName()] = token_pair;
        std::vector<std::string> identifiers = container->getIdentifiers();
        for( size_t i = 0; i < identifiers.size(); i++){
            parser_impl->identifiers[identifiers[i]] = token_pair;
        }

        return id;
    }

    void Parser::set_error(const std::string& error_string){
        parser_impl->error_description = error_string;
    }

    Parser::ParserImpl::ParserImpl() :
        error_description("")
        { };

    Parser::ParserImpl::~ParserImpl(){
        // Iterates through the registries, deleting containers as they appear.
        for (size_t i = 0; i < registered_options.size(); i++)
        {
            delete registered_options[i];
        };
        for (size_t i = 0; i < registered_positionals.size(); i++)
        {
            delete registered_positionals[i];
        };
    };
}
