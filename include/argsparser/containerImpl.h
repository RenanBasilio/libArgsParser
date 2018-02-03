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
            
            /* The following parameters are set during parsing and validation. */

            // Whether the parameter is active
            bool isActive;
            // The value of the parameter
            std::string value;
            // The pointer to the value of the parameter after post-processing
            void* post_result;
            
            // If validation succeeds, this is set to true and the failure reason is left empty.
            // If validation is critical and fails, exception is thrown and not handled.
            // If validation is not critical and fails, exception is thrown and handled, with 
            // the message recorded in the failure reason.
            bool validation_succeeded;
            std::string validation_failure_reason;
    };
}