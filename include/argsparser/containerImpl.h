/**
 * containerImpl.h
 * 
 * This file contains the declaration of the internals of the container class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/container.h>

namespace ArgsParser
{
    struct Container::ContainerImpl{
        // The name of the option
        const char* name;
        // The type of the option
        const ArgType option_type;
        // If this is a parameter option, the maximum and minimum number of arguments it can take
        const size_t min_values;
        const size_t max_values;
        // The vector of identifiers registered to this argument.
        const std::vector<std::string> identifiers;
        // The placeholder (if this is a parameter option) to display when generating help text
        const char* placeholder;
        // The description to display when generating help text
        const char* desc;
        // Whether an exception or failure from the validator should be considered a critical failure
        const bool validation_critical;
        // The function to be used to validate the option
        const Validator validator;
        // The function to be used as a callback method if validation succeeds.
        const Callback callback;
        // The function to be used as a callback method if validation fails.
        const Callback error_callback;
        
        /* The following parameters are set during parsing and validation. */

        // Whether the parameter is active
        bool isActive = false;
        // The values passed for the parameter. If multiple values are passed
        // they will be validated one at a time.
        std::vector<std::string> value;
        
        // If validation succeeds, this is set to true and the failure reason is left empty.
        // If validation is critical and fails, exception is thrown and not handled.
        // If validation is not critical and fails, exception is thrown and handled, with 
        // the message recorded in the failure reason.
        bool validation_succeeded = false;
        std::string validation_failure_reason = "";

        /**
         * Constructor for the Container structure.
         */
        // This constructor is used when a validator and postprocessor type function is provided.
        ContainerImpl(
            const char* &name,
            ArgsParser::ArgType type,
            size_t min_values,
            size_t max_values,
            const std::vector<std::string> &identifiers,
            const char* &placeholder, 
            const char* &desc,
            bool validation_critical,
            Validator validator,
            Callback error_callback,
            Callback callback) :
            name(name),
            option_type(type),
            min_values(min_values),
            max_values(max_values),
            identifiers(identifiers),
            placeholder(placeholder),
            desc(desc),
            validator(validator),
            validation_critical(validation_critical),
            callback(callback),
            error_callback(error_callback)
            {
            }
    };
}