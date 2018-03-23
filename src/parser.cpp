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
        no_except_(true),
        error_callback_(ArgsTools::print_error)
        { }

    Parser::Parser(bool no_except, ErrorHandler error_callback):
        parser_impl(new ParserImpl()),
        error_code(parser_impl->error_code),
        error_description(parser_impl->error_description),
        no_except_(no_except),
        error_callback_(no_except? error_callback : nullptr)
        { }

    Parser::Parser(const Parser& other):
        parser_impl(new ParserImpl()),
        error_code(parser_impl->error_code),
        error_description(parser_impl->error_description),
        no_except_(other.no_except_),
        error_callback_(other.error_callback_)
    { 
        for(auto var : parser_impl->names)
        {
            registerContainer(var.second.type, getContainer(var.second)->clone());
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

    std::string Parser::getProgramName() const noexcept {
        return parser_impl->program_name;
    }

    void Parser::setProgramName(const std::string& name) {
        parser_impl->program_name = name;
    }

    Token Parser::isRegistered(const std::string& symbol) const noexcept{
        return (isNameRegistered(symbol) || isIdentifierRegistered(symbol));
    }

    Token Parser::isNameRegistered(const std::string& name) const noexcept{
        return parser_impl->names.count(name) > 0? parser_impl->names[name] : NULL_TOKEN;
    }

    Token Parser::isIdentifierRegistered(const std::string& identifier) const noexcept{
        try {
            // First check if this is a proper identifier. If not, make one.
            const std::string identifier_ = 
                (ArgsTools::check_identifier(identifier)? identifier : ArgsTools::make_identifier(identifier));

            return parser_impl->identifiers.count(identifier_) > 0? parser_impl->identifiers[identifier_] : NULL_TOKEN;
        }
        catch (const std::exception&) { // Catch exception thrown by make_identifier if the identifier string is invalid.
            return NULL_TOKEN;
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
            identifiers_[i] = ArgsTools::make_identifier(identifiers[i]);
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
                    + std::to_string(std::numeric_limits<unsigned short>::max()) + std::string(")."));
            index = parser_impl->registered_positionals.size();
            parser_impl->registered_positionals.push_back(container);
            break;
        case ArgType::Option:
            if (parser_impl->registered_options.size() == (std::numeric_limits<unsigned short>::max()))
                throw new std::runtime_error("Exceeded maximum number of same type options (>" 
                    + std::to_string(std::numeric_limits<unsigned short>::max()) + std::string(")."));
            index = parser_impl->registered_options.size();
            parser_impl->registered_options.push_back(container);
            break;
        case ArgType::Switch:
            if (parser_impl->registered_switches.size() == (std::numeric_limits<unsigned short>::max()))
                throw new std::runtime_error("Exceeded maximum number of same type options (>" 
                    + std::to_string(std::numeric_limits<unsigned short>::max()) + std::string(")."));
            index = parser_impl->registered_switches.size();
            parser_impl->registered_switches.push_back(container);
            break;
        default:
            return NULL_TOKEN;
        }

        // Calculate the id of the container.
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

    const Container* Parser::getContainer(const std::string& name) const noexcept{
        return getContainer(isRegistered(name));
    }

    const Container* Parser::getContainer(const Token& token) const noexcept{

        return const_cast<Container*>(parser_impl->getContainer(token));

    }

    void Parser::parse(int argc, char* argv[]){
        std::string program_name = std::string(argv[0]);
        program_name = program_name.substr(program_name.find_last_of("/\\")+1);
        if (getProgramName() == "") setProgramName(program_name);

        std::vector<Container*> activeContainers;
        InputContainer* currentContainer = nullptr;
        size_t positional = 0;
        for (int i = 1; i < argc; i++)
        {
            std::string current = std::string(argv[i]);
            // First check whether we are dealing with an option or value.
            if(current.size() > 1) {
                if(current.at(0) == '-') // This is an option
                {
                    // Load the container for the option and set it to active.
                    Container* baseContainer = parser_impl->getContainer(isIdentifierRegistered(current));
                    if(baseContainer) {
                        baseContainer->setActive();
                        activeContainers.push_back(baseContainer);
                        // If container is not a switch, cast it to an input container and keep it loaded.
                        if(baseContainer->getType() != ArgType::Switch) 
                            currentContainer = dynamic_cast<InputContainer*>(baseContainer);
                    }
                }
                else // This is a value
                {
                    // If there is no current container, this is a positional, so load the container.
                    if(currentContainer == nullptr && (size_t)positional < parser_impl->registered_positionals.size()) {
                        currentContainer = dynamic_cast<InputContainer*>(
                            parser_impl->getContainer({ArgType::Positional, (unsigned short)positional}));
                            activeContainers.push_back(currentContainer);
                    }
                    
                    // If a container is loaded
                    if(currentContainer != nullptr)
                    {
                        // If this is a value option and the maximum amount of options have been loaded, throw an exception.
                        if(currentContainer->getType() == ArgType::Option && 
                        currentContainer->getInputSize() == currentContainer->getMaxInputs())
                            throw std::runtime_error("Too many arguments to option " + currentContainer->getName());

                        // Append the current input to the container.
                        currentContainer->setActive(current);

                        // If the current container is a positional argument, increment the positional counter and unload it.
                        if(currentContainer->getType() == ArgType::Positional)
                        {
                            positional++;
                            currentContainer = nullptr;
                        }
                        // Otherwise, it is an option, so unload it if it has read the maximum amount of inputs
                        else
                        {
                            if(currentContainer->getInputSize() == currentContainer->getMaxInputs()) 
                                currentContainer = nullptr;
                        }
                    }
                }
            }
        }
        for (size_t i = 0; i < activeContainers.size(); i++) {
            activeContainers[i]->postProcess();
        };
    };

    ValueWrapper Parser::getValue(const std::string& name) const noexcept{
        return getValue(isRegistered(name));
    };

    ValueWrapper Parser::getValue(const Token& token) const noexcept{
        const InputContainer* container = dynamic_cast<const InputContainer*>(getContainer(token));
        if (container != nullptr) return container->getValue();
        else return {std::vector<std::string>(), container->isActive()};
    };

    Token Parser::registerPositional(
        const std::string& name,
        const std::string& placeholder_text,
        const Validator<std::string>& validator,
        const ErrorHandler& error_callback,
        const Callback& callback
    ){
        return registerPositional<std::string>(
            name,
            placeholder_text,
            nullptr,
            validator,
            error_callback,
            callback
        );
    };

    Token Parser::registerOption(
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& placeholder_text,
        const std::string& description,
        const size_t max_values,
        const Validator<std::string>& validator,
        const ErrorHandler& error_callback,
        const Callback& callback
    ){
        return registerOption<std::string>(
            name,
            identifiers,
            placeholder_text,
            description,
            max_values,
            nullptr,
            validator,
            error_callback,
            callback
        );
    };

    Container* Parser::ParserImpl::getContainer(const Token& token) {
        try
        {
            switch (token.type)
            {
                case ArgType::Option:
                    return registered_options.at(token.position);
                    break;
                case ArgType::Positional:
                    return registered_positionals.at(token.position);
                    break;
                case ArgType::Switch:
                    return registered_switches.at(token.position);
                    break;
                default:
                    throw std::runtime_error("Container not found (NULL_TOKEN).");
                    break;
            }      
        }
        catch (const std::exception& e)
        {
            error_description = e.what();
            return nullptr;
        }
    };
}
