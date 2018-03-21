/**
 * user_input_container.h
 * 
 * This file contains the declaration of the UserInputContainer class, used to
 * process arguments where the input does not have a user-specified type and
 * is therefore treated as a string.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once
#include <argsparser/container.h>

namespace ArgsParser
{
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
}