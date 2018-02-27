/**
 * parser.h
 * 
 * This file contains the class declaration for the base Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <unordered_map>
#include <limits>
#include <argsparser/autohelp.h>
#include <argsparser/container.h>

namespace ArgsParser
{ 
    /**
     * This is the base Parser class. It provides basic parsing and auto-help
     * without additional features such as parameter validation.
     */
    class Parser{
        private:
            struct ParserImpl;
            std::unique_ptr<ParserImpl> parser_impl;

            // Whether errors should throw unhandled exceptions.
            bool errors_critical;

            // The following private methods are used to interface with the
            // implementation class.
            
            /**
             * This method registers a container of a specified type and returns
             * its id token.
             * 
             * @param {ArgType} type The type of option in the container.
             * @param {Container*} container The container being registered.
             */
            token register_container(ArgType type, Container* container);

            /**
             * This method sets the error description to the message provided.
             * 
             * @param {std::string} message The error message.
             */
            void set_error(const std::string& message);

        public:
            Parser();
            Parser(bool autohelp, bool errors_critical = false);
            Parser(const Parser& other);        // Copy constructor.
            Parser(Parser&& other);             // Move constructor.
            Parser& operator=(Parser other);    // Assignment operator
            ~Parser();

            friend void swap(Parser& first, Parser& second);

            const std::string& error_description;

            /**
             * This method returns whetehr a name or identifier is registered to 
             * the parser.
             * @param {std::string} symbol The symbol to search for.
             * @return {bool} True if symbol is registered. False otherwise.
             */
            bool isRegistered(const std::string& symbol);

            /**
             * This method returns whether a name is registered to the parser.
             * @param {std::string} name The name to search for.
             * @return {bool} True if name is registered. False otherwise.
             */
            bool isNameRegistered(const std::string& name);

            /**
             * This method returns whether an identifier is registered to the 
             * parser.
             * @param {std::string} identifier The identifier to search for.
             * @return {bool} True if the identifier is registered. False otherwise.
             */
            bool isIdentifierRegistered(const std::string& identifier);

            /**
             * This method register a positional argument to the parser. Usage example:
             *      ArgsParser.register_positional(
             *          "file", "filename", file_exists, show_error, callback);
             * 
             * The above line will enable parsing the following lines:
             *      myapp example.txt
             * The parser will then call file_exists(example.txt) and if true then callback(),
             * otherwise show_error().
             * 
             * It will generate the following help text:
             *      usage: myapp [<filename>]
             * 
             * Positionals are parsed in the order they are registered.
             * 
             * The method returns true if the option was registered successfully. Otherwise
             * it returns false and sets the "error_description" variable to a description of the
             * error.
             * 
             * @param {std::string} name The name of the positional.
             * @param {std::string} placeholder_text The text to show in a help message to describe the positional.
             * @param {Validator} validator The method to use to validate the user input string.
             * @param {Converter} converter The converter method to use for converting string input to type T.
             * @param {Callback} error_callback A method to call in case validation fails.
             * @param {Callback} callback A method to call in case validation succeeds.
             * @return {token} The token to retrieve the argument value by Id.
             * @except {std::runtime_error} Registration failure.
             */
            template <typename T>
            token register_positional(
                const std::string& name,
                const std::string& placeholder_text,
                const Validator& validator = nullptr,
                const Converter<T>& converter = nullptr,
                const Callback& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                try{
                    // First check if name is already registered.
                    if(isNameRegistered(name))
                        throw std::runtime_error("Name \"" + name + "\" is already registered.");

                    TypedInputContainer<T>* container = new TypedInputContainer<T>(
                        name,
                        std::vector<std::string>(),
                        "",
                        placeholder_text,
                        validator,
                        converter,
                        error_callback,
                        callback
                    );

                    token id = register_container(ArgType::Positional, container);

                    return id;
                }
                catch (const std::exception& e){
                    std::string error_string = std::string("Registration Error: ") + e.what();

                    if(errors_critical) throw std::runtime_error(error_string);
                    else set_error(error_string);

                    return null_token;
                }
            }
            token register_positional(
                const std::string& name,
                const std::string& placeholder_text,
                const Validator& validator = nullptr,
                const Callback& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                return register_positional<std::string>(
                    name,
                    placeholder_text,
                    validator,
                    nullptr,
                    error_callback,
                    callback
                );
            }

            /**
             * This method registers a switch to the parser. Usage example:
             *      Parser.register_switch(
             *          "help", {"h", "help"}, "Outputs help text to the command line.", autohelp);
             * 
             * The above line will enable parsing the following lines:
             *      myapp -h
             *      myapp --help
             * Upon reading either switch, the parser will call it's callback method.
             * 
             * Autohelp will generate the following help text:
             *      -h, --help          Outputs help text to the command line.
             * 
             * The method returns true if the option was registered successfully.
             * Otherwise it returns false and sets the "error_description" 
             * variable to a description of the error.
             * 
             * @param {std::string} name The name of the option.
             * @param {std::vector<std::string>} identifiers The identifiers to register with this option.
             * @param {std::string} description The description of the option to use for help text.
             * @param {Callback} callback A function to call upon encountering this switch.
             * @return {token} The token to retrieve the argument value by Id.
             * @except {std::runtime_error} Registration failure.
             */
            token register_switch(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "Description not given.",
                const Callback& callback = nullptr
            );

            /**
             * This method registers an input value option to the parser. Usage example:
             *      Parser.register_option(
             *          "file", {"f", "file"}, "filename", "The file to open", file_exists show_error, callback);
             * 
             * The above line will enable parsing the following lines:
             *      myapp -f example.txt
             *      myapp --file example.txt
             * The parser will then call file_exists(example.txt) and if true then callback(), otherwise
             * then show_error().
             * 
             * And generate the following help text:
             *      -f, --file <filename>      The file to open.
             * 
             * The method returns true if the option was registered successfully. Otherwise
             * it returns false and sets the "error_description" variable to a description of the
             * error.
             * 
             * @param {std::string} name The name of the option.
             * @param {std::vector<std::string>} identifiers The identifiers to register for this option
             * @param {std::string} placeholder_text The text that will be displayed within <> in the help text.
             * @param {std::string} description The description of the option that will be displayed in the help text.
             * @param {Validator} validator A function to check if the parameter provided with the option is valid.
             * @param {Converter} converter The converter method to use for converting string input to type T.
             * @param {Callback} error_callback A function to call if validation fails.
             * @param {Callback} callback A function to be called if validation succeeds.
             * @return {token} The token to retrieve the argument value by Id.
             * @except {std::runtime_error} Registration failure.
             */
            template <typename T>
            token register_option(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& placeholder_text = "value",
                const std::string& description = "Description not given.",
                const size_t max_values = 1,
                const size_t min_values = 1,
                const Validator& validator = nullptr,
                const Converter<T>& converter = nullptr,
                const Callback& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                try{
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
                
                    // If check was successful, create a new container object.
                    TypedInputContainer<T>* container = new TypedInputContainer<T>(
                        name,
                        identifiers_,
                        description,
                        placeholder_text,
                        validator,
                        converter,
                        error_callback,
                        callback
                    );

                    // And push the container to the array of registered options.
                    token id = register_container(ArgType::Option, container);

                    return id;
                }
                catch (const std::exception& e) {
                    std::string error_string = std::string("Registration Error: ") + e.what();

                    if(errors_critical) throw std::runtime_error(error_string);
                    else set_error(error_string);

                    return null_token;
                }
            }
            token register_option(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& placeholder_text = "value",
                const std::string& description = "Description not given.",
                const size_t max_values = 1,
                const size_t min_values = 1,
                const Validator& validator = nullptr,
                const Callback& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                return register_option<std::string>(
                    name,
                    identifiers,
                    placeholder_text,
                    description,
                    max_values,
                    min_values,
                    validator,
                    nullptr,
                    error_callback,
                    callback
                );
            }

            /**
             * This method returns a vector of all tokens that have been
             * registered to the parser.
             * @return {std::vector<token>} The list of registered tokens.
             */
            std::vector<token> get_registered_tokens();

            /**
             * This method returns a vector of all names that have been
             * registered to the parser.
             * @return {std::vector<std::string>} The list of registered names.
             */
            std::vector<std::string> get_registered_names();

            /**
             * This method gets a registered container by it's token.
             * @param {token} The registration token.
             * @return {Container} The container.
             */
            Container get_container_by_token(token token);

            /**
             * Calling this method will register the 'h' and 'help' switches under 'help'.
             * 
             * This method will return true if the switches registered successfully. Otherwise,
             * if one or both have already been registered, it will return false.
             * 
             * Alternatively, if you wish to register your own help provider, register a
             * switch with the method you wish to use as the callback function.
             * 
             * @return {bool} Whether autohelp was enabled successfully.
             */
            bool enable_autohelp();
    };
}