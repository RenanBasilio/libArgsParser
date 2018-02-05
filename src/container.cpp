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
        // The vector of identifiers registered to this argument.
        const std::vector<const char*> identifiers;
        // The placeholder (if this is a parameter option) to display when generating help text
        const char* placeholder;
        // The description to display when generating help text
        const char* desc;
        // Whether an exception or failure from the validator should be considered a critical failure
        const bool validation_critical;
        // The function to be used to validate the option
        const Validator validator;
        // The function to be used as a postprocessor method if the option is passed
        const Postprocessor postprocessor;
        // The function to be used as a callback method if the option is passed
        const Callback callback;
        
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
            std::vector<const char*> identifiers,
            std::string placeholder, 
            std::string desc,
            bool validation_critical,
            Validator validator,
            Postprocessor postprocessor,
            Callback callback) :
            name(name.c_str()),
            identifiers(identifiers),
            placeholder(placeholder.c_str()),
            desc(desc.c_str()),
            validator(validator),
            validation_critical(validation_critical),
            postprocessor(postprocessor),
            callback(callback)
            {

            }
    };

    Container::Container(
        std::string name, 
        std::vector<const char*> identifiers,
        std::string placeholder, 
        std::string desc,
        bool validation_critical,
        Validator validator,
        Postprocessor postprocessor,
        Callback callback
        )
    {
        container_impl = new ContainerImpl(
            name,
            identifiers,
            placeholder, 
            desc,
            validation_critical, 
            validator,
            postprocessor,
            callback);
    }

    const char* Container::getName(){
        return container_impl->name;
    }

    std::vector<const char*> Container::getIdentifiers(){
        return container_impl->identifiers;
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

    Validator Container::getValidator(){
        return container_impl->validator;
    }

    Postprocessor Container::getPostprocessor(){
        return container_impl->postprocessor;
    }

    Callback Container::getCallback(){
        return container_impl->callback;
    }

    bool Container::hasValidator(){
        return (container_impl->validator != nullptr);
    }

    bool Container::hasPostprocessor(){
        return (container_impl->postprocessor != nullptr);
    }

    bool Container::hasCallback(){
        return (container_impl->callback != nullptr);
    }
}