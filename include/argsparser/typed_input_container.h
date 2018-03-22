/**
 * typed_input_container.h
 * 
 * This file contains the definition of the template container class
 * TypedInputContainer, used to process argument values when the argument type
 * can be specified.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once
#include <argsparser/input_container.h>

namespace ArgsParser
{

    /**
     * This class is a InputContainer with an assigned type. It is to be
     * used when a specific return type is desired from an option, if said
     * type is known.
     * A converter function must be provided that transforms a string into the
     * desired type.
     * This class is used by positionals and input parameters.
     */
    template <typename T>
    class TypedInputContainer : public InputContainer {
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
            ) : InputContainer(
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