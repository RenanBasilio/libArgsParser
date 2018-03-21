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
#include <argsparser/value_wrapper.h>
#include <argsparser/common.h>

namespace ArgsParser
{
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
             * 
             * @return {string} The name of the argument stored in this container.
             */
            std::string getName() const;

            /**
             * This method retrieves the type of this container.
             * 
             * @return {ArgType} The type of the argument stored in this container.
             */
            ArgType getType() const;

            /**
             * This method retrieves the description assigned to this container.
             * 
             * @return {string} The description of the argument stored in this container.
             */
            std::string getDescription() const;
            
            /**
             * This method retrives the list of identifiers registered to this
             * container.
             * 
             * @return {vector<string>} The list of identifiers associated with this container.
             */
            std::vector<std::string> getIdentifiers() const;

            /**
             * This method retrieves whether this container is active. This is
             * always false before parsing, and will be set if this container
             * refers to an option called by the user.
             * 
             * @return {bool} Whether the parser found this container in the command line.
             */
            bool isActive() const;

            /**
             * Constructor of the container class.
             * 
             * @param {ArgType} type The type of the argument.
             * @param {string} name The name of the argument.
             * @param {vector<string>} identifiers The list of identifiers associated with the argument.
             * @param {string} description The description of the argument.
             * @param {Callback} callback The method to call during post processing if this argument is found by the parser.
             */
            Container(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description,
                const Callback& callback
            );

            /**
             * Destructor of the container class.
             */
            virtual ~Container(); 

            /**
             * Cloning method.
             * 
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

            /**
             * This method sets the container to active. 
             * It is private, and can only be called by the parser. It will be
             * called if the parser finds this argument in the command line.
             */
            void setActive();

            /**
             * This method executes all post-processing logic associated with 
             * this container.
             * It is virtual, as the derived classes use different post-processing
             * logic to account for features such as validation or conversion. 
             */
            virtual void postProcess();

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
             * 
             * @return {string} The placeholder text associated with this input argument.
             */
            virtual std::string getPlaceholderText() const;

            /**
             * This method retrieves user input how it was parsed from the
             * command line. Only string conversion is performed.
             * 
             * @return {vector<string>} The list of user inputs parsed to this container.
             */
            std::vector<std::string> getUserInput() const;

            /**
             * This method returns the number of independent inputs parsed to this
             * container.
             * 
             * @return {size_t} The size of the list of user inputs parsed to this container.
             */
            size_t getInputSize();

            /**
             * This method returns the maximum number of independent inputs that this
             * container can hold.
             * 
             * @return {size_t} The maximum number of inputs that this container can hold.
             */
            size_t getMaxInputs();

            /**
             * This method returns the wrapped value of this container.
             * 
             * @return {ValueWrapper} The wrapped state and input stored in this container.
             */
            ValueWrapper getValue() const;

            /**
             * This method returns the validation state of the user input as a
             * pair, with the first element being whether the validation was
             * successful and the second being the error string generated.
             * 
             * @return {pair<bool, string>} The validation state and error message (if failure).
             */
            std::pair<bool, std::string> getValidation() const;

            /**
             * This is the constructor for the user input container.
             * 
             * @param {ArgType} type The type of argument stored in this container.
             * @param {string} name The name of the argument stored in this container.
             * @param {vector<string>} identifiers The identifiers associated with the argument stored in this container.
             * @param {string} description The description of the argument stored in this container.
             * @param {string} placeholder_text The placeholder text of the argument stored in this container.
             * @param {size_t} max_values The maximum number of inputs this container can hold.
             * @param {Validator} validator A function to use for input validation.
             * @param {ErrorHandler} error_callback A function to call if validation fails.
             * @param {Callback} callback A function to call if validation succeeds.
             */
            UserInputContainer(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const std::string& placeholder_text = "",
                const size_t max_values = 1,
                const Validator<std::string>& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            );

            /**
             * This is the destructor for the user input container.
             */
            virtual ~UserInputContainer();

            /**
             * Cloning method.
             * 
             * @return {Container*} A pointer to a new UserInputContainer object cloned from this object.
             */
            virtual UserInputContainer* clone() const;


        protected:
            const size_t max_values_;

            std::string placeholder_text_;
            std::vector<std::string> user_input_;

            const Validator<std::string> validator_;
            const ErrorHandler error_callback_;

            bool validation_;
            std::string validation_failure_reason_;

            friend class Parser;

            /**
             * This method sets the container state to active and appends the
             * input string to the list of user inputs. It will be called when
             * a value passed to the identifiers associated with this container
             * is found by the parser.
             * 
             * @param {string} user_input The input to associate with this container.
             */
            void setActive(const std::string& user_input);

            /**
             * This method executes all post-processing logic associated with 
             * this container.
             * 
             * This method will call the validator on each input, and if every
             * validation succeeds the callback function will be called. If even
             * a single validation fails the error callback will be called instead.
             * 
             * It is virtual, as the derived classes use different post-processing
             * logic to account for features such conversion. 
             */
            virtual void postProcess();
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
             * 
             * @return {T} The result of the conversion of the user input stored in this parser.
             */
            std::vector<T> getConvertedValue() const {
                return converted_value_;
            };

            /**
             * This is the constructor for the typed user input container.
             * 
             * @param {ArgType} type The type of argument stored in this container.
             * @param {string} name The name of the argument stored in this container.
             * @param {vector<string>} identifiers The identifiers associated with the argument stored in this container.
             * @param {string} description The description of the argument stored in this container.
             * @param {string} placeholder_text The placeholder text of the argument stored in this container.
             * @param {size_t} max_values The maximum number of inputs this container can hold.
             * @param {Validator} validator A function to use for input validation.
             * @param {Converter<T>} converter A function to use for input conversion.
             * @param {ErrorHandler} error_callback A function to call if validation fails.
             * @param {Callback} callback A function to call if validation succeeds.
             */
            TypedInputContainer(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description = "",
                const std::string& placeholder_text = "",
                const size_t max_values = 1,
                const Converter<T>& converter = nullptr,
                const Validator<T>& validator = nullptr,
                const ErrorHandler& error_callback = nullptr,
                const Callback& callback = nullptr
            ) : UserInputContainer(
                    type,
                    name, 
                    identifiers,
                    description,
                    placeholder_text,
                    max_values,
                    nullptr,
                    error_callback,
                    callback
                    ),
                converter_(converter),
                validator_(validator)
                {};

            /**
             * Cloning method.
             * 
             * @return {Container*} A pointer to a new TypedInputContainer object cloned from this object.
             */
            virtual TypedInputContainer<T>* clone() const
            {
                TypedInputContainer<T>* copy = new TypedInputContainer<T>(
                    type_,
                    name_,
                    identifiers_,
                    description_,
                    placeholder_text_,
                    max_values_,
                    converter_,
                    validator_,
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
            std::vector<T> converted_value_;

            const Converter<T> converter_;

            const Validator<T> validator_;     

            friend class Parser;

            /**
             * This method executes all post-processing logic associated with 
             * this container.
             * 
             * This method will call the converter on each input, and upon
             * successful conversion will call the validator. If every
             * validation succeeds the callback function will be called. If even
             * a single validation or conversion fails the error callback will 
             * be called instead.
             * 
             * It is virtual, as the derived classes use different post-processing
             * logic to account for other features.
             */
            virtual void postProcess() {
                if (converter_ != nullptr) {
                    for (size_t i = 0; i < user_input_.size(); i++) {
                        try {
                            T input = converter_(user_input_[i]);
                            if (validator_ != nullptr) {
                                bool valid = validator_(input);
                                if (!valid) throw std::runtime_error("Unspecified validation error.");
                            }

                            converted_value_.push_back(input);
                        }
                        catch (std::exception& e) {
                            validation_ = false;
                            validation_failure_reason_ = e.what();

                            if(error_callback_ != nullptr) error_callback_(e);
                            return;
                        }
                    }
                }
                if (callback_ != nullptr) callback_();
            };
    };
}