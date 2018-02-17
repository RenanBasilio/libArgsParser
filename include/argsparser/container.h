/**
 * container.h
 * 
 * This file contains the public interface of the container class used to store
 * data.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <argsparser/common.h>

namespace ArgsParser
{
    
    /**
     * This enumeration is used internally to define the possible types of
     * options and store them for use by the parser.
     */
    enum ArgType
    {
        Positional = 1,
        Switch,
        Option
    };

    /**
     * This template base class is used to store basic information about an
     * argument type to be parsed.
     * This class is used when a switch is registered, as it has a smaller
     * memory footprint.
     */
    class Container{
        protected:
            const std::string name_;
            const std::string description_;
            const std::vector<std::string> identifiers_;
            const Callback callback_;

            bool active_;

        public:
            /**
             * This method retrieves the name assigned to this container.
             */
            const std::string getName();

            /**
             * This method retrieved the description assigned to this container.
             */
            const std::string getDescription();
            
            /**
             * This method retrives the list of identifiers registered to this
             * container.
             */
            const std::vector<std::string> getIdentifiers();

            /**
             * This pure virtual method provides the facilities for retrieving
             * the placeholder text from a UserInputContainer through an object
             * of type Container.
             */
            virtual const std::string getPlaceholderText(){
                return "";
            };

            /**
             * This method retrieves whether this container is active. This is
             * always false before parsing, and will be set if this container
             * refers to an option called by the user.
             */
            const bool isActive();

            /**
             * Constructor of the container class.
             */
            Container(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const Callback& callback = nullptr
            );
    };


    /**
     * This class is used to parse for user inputted strings. It is based on 
     * the basic container class with additional methods to store and retrieve
     * input parameters.
     */
    class UserInputContainer : public Container{
        std::string placeholder_text_;

        public:
            /**
             * This method retrieves the placeholder text assigned to this
             * container. The placeholder text is used for generating help
             * text.
             */
            const std::string getPlaceholderText();

            /**
             * This method retrieves user input how it was parsed from the
             * command line. Only string conversion is performed.
             */
            const std::string getUserInput();

            /**
             * This method returns the validation state of the user input as a
             * pair, with the first element being whether the validation was
             * successful and the second being the error string generated.
             */
            const std::pair<bool, std::string> getValidation();

            /**
             * This is the constructor for the user input container.
             */
            UserInputContainer(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const std::string& placeholder_text = "",
                const Validator& validator = nullptr,
                const Callback& error_callback = nullptr,
                const Callback& callback = nullptr
            );


        protected:
            std::string user_input_;

            const Validator validator_;
            const Callback error_callback_;

            bool validation_;
            std::string validation_failure_reason_;
    };

    /**
     * This class is a UserInputContainer with an assigned type. It is to be
     * used when a specific return type is desired from an option, if said
     * type is known.
     * A converter function must be provided that transforms a string into the
     * desired type.
     * This class is used by positionals and input parameters.
     */
    template <typename T>
    class TypedInputContainer : public UserInputContainer {
        public:    
            /**
             * This method gets the converted value of the user input.
             */
            const T getValue() {
                if (converter_ != nullptr) return converted_value_;
                else throw std::runtime_error("Warning: Converter undefined. To retrieve string input use getUserInput() instead.");
            };

            /**
             * This is the constructor for the typed user input container.
             */
            TypedInputContainer(
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const std::string& placeholder_text = "",
                const Validator& validator = nullptr,
                const Converter<T>& converter = nullptr,
                const Callback& error_callback = nullptr,
                const Callback& callback = nullptr
            ) : UserInputContainer(
                    name, 
                    identifiers,
                    description,
                    placeholder_text, 
                    validator,
                    error_callback,
                    callback
                    ),
                converter_(converter)
                {};

        private:
            T converted_value_;

            const Converter<T> converter_;     
    };
}