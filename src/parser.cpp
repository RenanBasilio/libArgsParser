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
        error_code(parser_impl->error_code),
        error_description(parser_impl->error_description),
        errors_critical(true)
        { }

    Parser::Parser(bool autohelp, ErrorHandler error_callback):
        parser_impl(new ParserImpl()),
        error_code(parser_impl->error_code),
        error_description(parser_impl->error_description),
        errors_critical(error_callback? false : true),
        error_callback(error_callback)
    {
        if(autohelp) enableAutohelp();
    }

    Parser::Parser(const Parser& other):
        parser_impl(new ParserImpl()),
        error_code(parser_impl->error_code),
        error_description(parser_impl->error_description),
        errors_critical(other.errors_critical),
        error_callback(other.error_callback)
    { 
        for(auto var : parser_impl->names)
        {
            registerContainer(var.second.type, getContainerByToken(var.second).release());
        }
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
        parser_impl.reset();
    }


    Parser::ParserImpl::ParserImpl() :
        error_description(""),
        error_code(0)
        { };

    Parser::ParserImpl::~ParserImpl(){
        // Iterates through the registries, deleting containers as they appear.
        for (size_t i = 0; i < registered_options.size(); i++)
        {
            delete registered_options[i];
            registered_options[i] = nullptr;
        };
        for (size_t i = 0; i < registered_positionals.size(); i++)
        {
            delete registered_positionals[i];
            registered_positionals[i] = nullptr;
        };
    };

    void swap(Parser& first, Parser& second){
        using std::swap;

        std::swap(first.parser_impl, second.parser_impl);
    }

    std::string Parser::getProgramName() const{
        return parser_impl->program_name;
    }

    void Parser::setProgramName(const std::string& name){
        parser_impl->program_name = name;
    }

    Token Parser::isRegistered(const std::string& symbol) const{
        return (isNameRegistered(symbol) || isIdentifierRegistered(symbol));
    }

    Token Parser::isNameRegistered(const std::string& name) const{
        return parser_impl->names.count(name) > 0? parser_impl->names[name] : NULL_TOKEN;
    }

    Token Parser::isIdentifierRegistered(const std::string& identifier) const{
        // First check if this is a proper identifier. If not, make one.
        const std::string identifier_ = 
            (check_identifier(identifier)? identifier : make_identifier(identifier));

        return parser_impl->identifiers.count(identifier_) > 0? parser_impl->identifiers[identifier_] : NULL_TOKEN;
    }

    bool Parser::enableAutohelp(){
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

    Token Parser::registerSwitch(
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
            ArgType::Switch,
            name,
            identifiers_,
            description,
            callback
        );

        Token id = registerContainer(ArgType::Switch, container);

        return id;
    }

    Token Parser::registerContainer(ArgType type, Container* container){
        // The identifier type allows for up to 6553 options of each type to be
        // registered. As such, throw an exception if that amount is reached.

        // Register the container
        size_t index;
        switch (type)
        {
        case ArgType::Positional:
            if (parser_impl->registered_positionals.size() == (std::numeric_limits<unsigned short>::max()))
                throw new std::runtime_error("Exceeded maximum number of same type options (>" 
                    + std::numeric_limits<unsigned short>::max() + std::string(")."));
            index = parser_impl->registered_positionals.size();
            parser_impl->registered_positionals.push_back(container);
            break;
        case ArgType::Option:
            if (parser_impl->registered_options.size() == (std::numeric_limits<unsigned short>::max()))
                throw new std::runtime_error("Exceeded maximum number of same type options (>" 
                    + std::numeric_limits<unsigned short>::max() + std::string(")."));
            index = parser_impl->registered_options.size();
            parser_impl->registered_options.push_back(container);
            break;
        case ArgType::Switch:
            if (parser_impl->registered_switches.size() == (std::numeric_limits<unsigned short>::max()))
                throw new std::runtime_error("Exceeded maximum number of same type options (>" 
                    + std::numeric_limits<unsigned short>::max() + std::string(")."));
            index = parser_impl->registered_switches.size();
            parser_impl->registered_switches.push_back(container);
            break;
        default:
            return NULL_TOKEN;
        }

        // Calculate the id of the container.
        // The last digit of the container is the type (ArgType), while
        // the remaining digits are the index in the container.
        Token id_token =  {type, (unsigned short)index};

        // Add the id to the map of names.
        parser_impl->names[container->getName()] = id_token;
        std::vector<std::string> identifiers = container->getIdentifiers();
        for( size_t i = 0; i < identifiers.size(); i++){
            parser_impl->identifiers[identifiers[i]] = id_token;
        }

        return id_token;
    }

    void Parser::setError(const std::string& error_string){
        parser_impl->error_description = error_string;
    }

    std::vector<Token> Parser::getRegisteredTokens() const{
        std::vector<Token> tokens;
        for(auto var : parser_impl->names)
        {
            tokens.push_back(var.second);
        }
        return tokens;
    }

    std::vector<std::string> Parser::getRegisteredNames() const{
        std::vector<std::string> names;
        for(auto var : parser_impl->names)
        {
            names.push_back(var.first);
        }
        return names;
    }

    std::unique_ptr<Container> Parser::getContainerByToken(Token token) const{

        Container* result;
        switch (token.type)
        {
            case ArgType::Option:
                result = parser_impl->registered_options[token.position];
                break;
            case ArgType::Positional:
                result = parser_impl->registered_positionals[token.position];
                break;
            case ArgType::Switch:
                result = parser_impl->registered_switches[token.position];
                break;
            default:
                throw std::runtime_error("NULL_TOKEN presented.");
                break;
        }
        return std::unique_ptr<Container>(result->clone());
    };

    void Parser::Parse(int argc, char* argv[]){
        std::string program_name = std::string(argv[0]);
        program_name = program_name.substr(program_name.find_last_of("/\\"));
        if (getProgramName() == "") setProgramName(program_name);

        std::unique_ptr<Container> container = nullptr;
        int positional = 0;
        for (int i = 1; i < argc; i++)
        {
            std::string current = std::string(argv[i]);
            // First check whether we are dealing with an option or value.
            if(current.at(0) == '-') // This is an option
            {
                // Load the container for the option and set it to active.
                container = getContainerByToken(isIdentifierRegistered(current));
                container->setActive();
            }
            else // This is a value
            {

            }
        }
    };
}
