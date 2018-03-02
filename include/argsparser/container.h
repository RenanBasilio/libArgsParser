/**
 * container.h
 * 
 * This file contains the public interface of the container class used to store
 * data.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <algorithm>
#include <argsparser/util.h>

namespace ArgsParser
{
    struct ValueWrapper
    {
        std::string user_input;
        bool active;

        operator bool() {return active;};
        operator std::string() {return user_input;};
        operator const char*() {return user_input.c_str();};
    };

    /**
     * This template base class is used to store basic information about an
     * argument type to be parsed.
     * This class is used when a switch is registered, as it has a smaller
     * memory footprint.
     */
    class Container{
        public:
            /**
             * This method retrieves the name assigned to this container.
             */
            std::string getName() const;

            /**
             * This method retrieves the type of this container.
             */
            ArgType getType() const;

            /**
             * This method retrieves the description assigned to this container.
             */
            std::string getDescription() const;
            
            /**
             * This method retrives the list of identifiers registered to this
             * container.
             */
            std::vector<std::string> getIdentifiers() const;

            virtual std::string getPlaceholderText() const;

            virtual ValueWrapper getValue() const;

            virtual std::string getUserInput() const;

            /**
             * This method retrieves whether this container is active. This is
             * always false before parsing, and will be set if this container
             * refers to an option called by the user.
             */
            bool isActive() const;

            /**
             * Constructor of the container class.
             */
            Container(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const Callback& callback = nullptr
            );

            /**
             * Destructor of the container class.
             */
            virtual ~Container(); 

            /**
             * Cloning method.
             * @return {Container*} A pointer to a new Container object cloned from this object.
             */
            virtual Container* clone() const;

        protected:
            const std::string name_;
            const std::string description_;
            const std::vector<std::string> identifiers_;
            const Callback callback_;
            const ArgType type_;

            bool active_;

            friend class Parser;

            void setActive();

    };


    /**
     * This class is used to parse for user inputted strings. It is based on 
     * the basic container class with additional methods to store and retrieve
     * input parameters.
     */
    class UserInputContainer : public Container{
        public:
            /**
             * This method retrieves the placeholder text assigned to this
             * container. The placeholder text is used for generating help
             * text.
             */
            virtual std::string getPlaceholderText() const;

            /**
             * This method retrieves user input how it was parsed from the
             * command line. Only string conversion is performed.
             */
            std::string getUserInput() const;

            ValueWrapper getValue() const;

            /**
             * This method returns the validation state of the user input as a
             * pair, with the first element being whether the validation was
             * successful and the second being the error string generated.
             */
            std::pair<bool, std::string> getValidation() const;

            /**
             * This is the constructor for the user input container.
             */
            UserInputContainer(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const std::string& placeholder_text = "",
                const Validator& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            );

            /**
             * This is the destructor for the user input container.
             */
            virtual ~UserInputContainer();

            /**
             * Cloning method.
             * @return {Container*} A pointer to a new UserInputContainer object
             * cloned from this object.
             */
            virtual UserInputContainer* clone() const;


        protected:
            std::string placeholder_text_;
            std::string user_input_;

            const Validator validator_;
            const ErrorHandler error_callback_;

            bool validation_;
            std::string validation_failure_reason_;

            friend class Parser;
            void setActive(const std::string& user_input);
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
            T getConvertedValue() const;

            /**
             * This is the constructor for the typed user input container.
             */
            TypedInputContainer(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const std::string& placeholder_text = "",
                const Validator& validator = nullptr,
                const Converter<T>& converter = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            ) : UserInputContainer(
                    type,
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

            /**
             * Cloning method.
             * @return {Container*} A pointer to a new TypedInputContainer object
             * cloned from this object.
             */
            virtual TypedInputContainer<T>* clone() const
            {
                TypedInputContainer<T>* copy = new TypedInputContainer<T>(
                    type_,
                    name_,
                    identifiers_,
                    description_,
                    placeholder_text_,
                    validator_,
                    converter_,
                    error_callback_,
                    callback_
                );
                copy->user_input_ = user_input_;
                copy->validation_ = validation_;
                copy->validation_failure_reason_ = validation_failure_reason_;
                copy->converted_value_ = converted_value_;
                return copy;
            };

            /**
             * This is the destructor for the typed user input container.
             */
            virtual ~TypedInputContainer() {};

        private:
            T converted_value_;

            const Converter<T> converter_;     

            friend class Parser;
    };
}