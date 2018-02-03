/**
 * data_container.cpp
 * 
 * This file contains the implementations of the different data containers to
 * be used for the different kinds of parameters.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/parser.h>

namespace ArgsParser
{
    
    struct Parser::DataContainer{
        private:
            // Validator functions must take one pointer (the value of the parameter) and
            // return one boolean or throw an exception.
            typedef bool (*Validator) (const void *);

            // Callback functions must take one pointer (the value of the parameter) and
            // return void (as the library will not handle values returned by it). If a
            // return value is needed, consider using a messaging system.
            typedef void (*Callback) (const void *);

        public:
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
            
            // If validation succeeds, this is set to true and the failure reason is left empty.
            // If validation is critical and fails, exception is thrown and not handled.
            // If validation is not critical and fails, exception is thrown and handled, with 
            // the message recorded in the failure reason.
            bool validation_succeeded;
            std::string validation_failure_reason;


            /**
             * Constructor for the DataContainer structure.
             */
            DataContainer(std::string name, 
                char short_id, 
                std::string long_id, 
                std::string placeholder, 
                std::string desc,
                bool (*validator) (const void *),
                bool validation_critical,
                void (*callback) (const void *)
            ) : name(name.c_str()),
                short_id(short_id),
                long_id(long_id.c_str()), 
                placeholder(placeholder.c_str()),
                desc(desc.c_str()),
                validator(validator),
                validation_critical(validation_critical),
                callback(callback)
            {

            }
    };

    // To-Do: Separate validation options from DataContainer. Add them to separate ValidatingContainer instead.
}