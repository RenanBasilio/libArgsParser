/**
 * containerImpl.h
 * 
 * This file contains the implementations of the different data containers to
 * be used for the different kinds of parameters.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/common.h>

namespace ArgsParser
{
    
    class ContainerImpl{
        private:
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
            const Callback callback;
            const bool hasCallback;

            const Postprocessor postprocessor;
            const bool hasPostprocessor;
            
            /* The following parameters are set during parsing and validation. */

            // Whether the parameter is active
            bool isActive;
            // The value of the parameter
            std::string value;
            
            // If validation succeeds, this is set to true and the failure reason is left empty.
            // If validation is critical and fails, exception is thrown and not handled.
            // If validation is not critical and fails, exception is thrown and handled, with 
            // the message recorded in the failure reason.
            bool validation_succeeded;
            std::string validation_failure_reason;

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
                Postprocessor postprocessor = nullptr) :
                name(name.c_str()),
                short_id(short_id),
                placeholder(placeholder.c_str()),
                desc(desc.c_str()),
                validator(validator),
                validation_critical(validation_critical),
                callback(nullptr),
                hasCallback(false),
                postprocessor(postprocessor),
                hasPostprocessor(postprocessor != nullptr ? true : false)
                {

                }
                
            // This constructor is used when a validator and callback type function is provided.
            ContainerImpl(std::string name, 
                char short_id, 
                std::string long_id, 
                std::string placeholder, 
                std::string desc,
                Validator validator = nullptr,
                bool validation_critical = true,
                Callback callback = nullptr) :
                name(name.c_str()),
                short_id(short_id),
                placeholder(placeholder.c_str()),
                desc(desc.c_str()),
                validator(validator),
                validation_critical(validation_critical),
                callback(nullptr),
                hasCallback(false),
                postprocessor(postprocessor),
                hasPostprocessor(postprocessor != nullptr ? true : false)
                {

                }
    };
}