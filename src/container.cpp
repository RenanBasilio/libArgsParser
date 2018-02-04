/**
 * container.h
 * 
 * This file contains the implementation of methods in the Container class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/container.h>

namespace ArgsParser
{
    struct Container::ContainerImpl{
        /* The following members are constant. Once set by the constructor they cannot be changed. */

        // The name of the option
        const char* name;
        // The short single-character identifier
        const char short_id;
        // The long multi-character identifier
        const char* long_id;
        // The placeholder (if this is a parameter option) to display when generating help text
        const char* placeholder;
        // The description to display when generating help text
        const char* desc;
        // Whether an exception or failure from the validator should be considered a critical failure
        const bool validation_critical;
        // The function to be used to validate the option
        const Validator validator;
        // The function to be used as a callback method if the option is passed
        const Postprocessor callback;
        
        /* The following parameters are set during parsing and validation. */

        // Whether the parameter is active
        bool isActive = false;
        // The value of the parameter
        std::string value = "";
        
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
        ContainerImpl(std::string name, 
            char short_id, 
            std::string long_id, 
            std::string placeholder, 
            std::string desc,
            Validator validator = nullptr,
            bool validation_critical = true,
            Postprocessor callback = nullptr) :
            name(name.c_str()),
            short_id(short_id),
            placeholder(placeholder.c_str()),
            desc(desc.c_str()),
            validator(validator),
            validation_critical(validation_critical),
            callback(callback)
            {

            }
    };

    Container::Container(
        std::string name, 
        char short_id, 
        std::string long_id, 
        std::string placeholder, 
        std::string desc,
        Validator validator,
        bool validation_critical,
        Postprocessor callback)
    {
        container_impl = new ContainerImpl(
            name,
            short_id, 
            long_id, 
            placeholder, 
            desc, 
            validator, 
            validation_critical, 
            callback);
    }

    const char* Container::getName(){
        return container_impl->name;
    }

    const char Container::getShortId(){
        return container_impl->short_id;
    }

    const char* Container::getLongId(){
        return container_impl->long_id;
    }

    const char* Container::getPlaceholderText(){
        return container_impl->placeholder;
    }

    const char* Container::getDescription(){
        return container_impl->desc;
    }

    std::pair<bool, std::string> Container::getValue(){
        return std::make_pair(container_impl->isActive, container_impl->value);
    }

    std::pair<bool, std::string> Container::getValidation(){
        return std::make_pair(container_impl->validation_succeeded, container_impl->validation_failure_reason);
    }
}