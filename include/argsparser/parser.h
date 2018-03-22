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
#include <argsparser/typed_input_container.h>
#include <argsparser/util.h>

namespace ArgsParser
{ 
    /**
     * This is the base Parser class. It provides basic parsing and auto-help
     * without additional features such as parameter validation.
     */
    class Parser{
        struct ParserImpl;
        std::unique_ptr<ParserImpl> parser_impl;

        // Whether errors should throw unhandled exceptions.
        const bool no_except_;

        // A method to call if an error occurs.
        ErrorHandler error_callback_;
        
        public:
            Parser();
            Parser(bool no_except, ErrorHandler error_callback = ArgsTools::print_error);
            Parser(const Parser& other);        // Copy constructor.
            Parser(Parser&& other);             // Move constructor.
            Parser& operator=(Parser other);    // Assignment operator
            ~Parser();

            friend void swap(Parser& first, Parser& second);

            // Accessor Operators
            ValueWrapper operator[](const std::string& name) const;
            ValueWrapper operator[](const Token& token) const;
            template <typename T> TypedValueWrapper<T> operator[](const std::string& name) const
            {
                return getValue<T>(name);
            };
            template <typename T> TypedValueWrapper<T> operator[](const Token& token) const
            {
                return getValue<T>(token);
            };

            ValueWrapper getValue(const std::string& name) const;
            ValueWrapper getValue(const Token& token) const;
            template <typename T> TypedValueWrapper<T> getValue(const std::string& name) const
            {
                return getValue<T>(isRegistered(name));
            };
            template <typename T> TypedValueWrapper<T> getValue(const Token& token) const
            {
                TypedInputContainer<T>* container = dynamic_cast<TypedInputContainer<T>*>(getContainer(token));
                return container->getConvertedValue();
            };

            const unsigned short& error_code;
            const std::string& error_description;

            /**
             * This method returns whether a name or identifier is registered to 
             * the parser.
             * @param {std::string} symbol The symbol to search for.
             * @return {bool} True if symbol is registered. False otherwise.
             */
            Token isRegistered(const std::string& symbol) const;

            /**
             * This method returns whether a name is registered to the parser.
             * @param {std::string} name The name to search for.
             * @return {bool} True if name is registered. False otherwise.
             */
            Token isNameRegistered(const std::string& name) const;

            /**
             * This method returns whether an identifier is registered to the 
             * parser.
             * @param {std::string} identifier The identifier to search for.
             * @return {bool} True if the identifier is registered. False otherwise.
             */
            Token isIdentifierRegistered(const std::string& identifier) const;

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
             * @return {Token} The token to retrieve the argument value by Id.
             * @except {std::runtime_error} Registration failure.
             */
            template <typename T>
            Token registerPositional(
                const std::string& name,
                const std::string& placeholder_text,
                const Converter<T>& converter = nullptr,
                const Validator<T>& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                try{
                    // First check if name is already registered.
                    if(isNameRegistered(name))
                        throw std::runtime_error("Name \"" + name + "\" is already registered.");

                    TypedInputContainer<T>* container = new TypedInputContainer<T>(
                        ArgType::Positional,
                        name,
                        std::vector<std::string>(),
                        "",
                        placeholder_text,
                        1,
                        converter,
                        validator,
                        error_callback,
                        callback
                    );

                    Token id = registerContainer(ArgType::Positional, container);

                    return id;
                }
                catch (const std::exception& e){
                    std::string error_string = std::string("Registration Error: ") + e.what();

                    if(!no_except_) throw std::runtime_error(error_string);
                    else setError(error_string);

                    return NULL_TOKEN;
                }
            }
            Token registerPositional(
                const std::string& name,
                const std::string& placeholder_text,
                const Validator<std::string>& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                return registerPositional<std::string>(
                    name,
                    placeholder_text,
                    nullptr,
                    validator,
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
             * @return {Token} The token to retrieve the argument value by Id.
             * @except {std::runtime_error} Registration failure.
             */
            Token registerSwitch(
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
             * @return {Token} The token to retrieve the argument value by Id.
             * @except {std::runtime_error} Registration failure.
             */
            template <typename T>
            Token registerOption(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& placeholder_text = "value",
                const std::string& description = "Description not given.",
                const size_t max_values = 1,
                const Converter<T>& converter = nullptr,
                const Validator<T>& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            ){
                try{
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
                
                    // If check was successful, create a new container object.
                    TypedInputContainer<T>* container = new TypedInputContainer<T>(
                        ArgType::Option,
                        name,
                        identifiers_,
                        description,
                        placeholder_text,
                        max_values,
                        converter,
                        validator,
                        error_callback,
                        callback
                    );

                    // And push the container to the array of registered options.
                    Token id = registerContainer(ArgType::Option, container);

                    return id;
                }
                catch (const std::exception& e) {
                    std::string error_string = std::string("Registration Error: ") + e.what();

                    if(!no_except_) throw std::runtime_error(error_string);
                    else setError(error_string);

                    return NULL_TOKEN;
                }
            }
            Token registerOption(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& placeholder_text = "value",
                const std::string& description = "Description not given.",
                const size_t max_values = 1,
                const Validator<std::string>& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
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
            }

            /**
             * This method returns a vector of all tokens that have been
             * registered to the parser.
             * @return {std::vector<Token>} The list of registered tokens.
             */
            std::vector<Token> getRegisteredTokens() const;

            /**
             * This method returns a vector of all names that have been
             * registered to the parser.
             * @return {std::vector<std::string>} The list of registered names.
             */
            std::vector<std::string> getRegisteredNames() const;

            /**
             * This method gets a registered container by it's token.
             * @param {Token} The registration token.
             * @return {Container*} The container.
             */
            const Container* getContainer(const std::string& name) const;
            const Container* getContainer(const Token& token) const;

            /**
             * This method gets the program name (either as provided by the user
             * or parsed by the application).
             * @return {std::string} The name of the program.
             */
            std::string getProgramName() const;

            /**
             * This method sets the program name. If a name is set it will override
             * one parsed from the command line.
             */
            void setProgramName(const std::string& name);

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
            bool enableAutohelp();

            /**
             * This method parses argv.
             * @param {int} argc The argument count.
             * @param {char**} argv The argument vector.
             */
            void parse(int argc, char* argv[]);

        private:
            // The following private methods are used to interface with the
            // implementation class.
            
            /**
             * This method registers a container of a specified type and returns
             * its id token.
             * 
             * @param {ArgType} type The type of option in the container.
             * @param {Container*} container The container being registered.
             */
            Token registerContainer(ArgType type, Container* container);

            /**
             * This method sets the error description to the message provided.
             * 
             * @param {std::string} message The error message.
             */
            void setError(const std::string& message);

            /**
             * This method sets the container represented by a token to active and
             * optionally assigns it a user input parameter.
             * 
             * @param {Token} token The token that identifies the option.
             * @param {std::string} user_input The user input data.
             */
            void setActive(const Token token, const std::string& user_input = nullptr);
    };
}