/**
 * container.h
 * 
 * This file contains the public interface of the container class used to store
 * data.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <argsparser/common.h>

namespace ArgsParser
{
    
    /**
     * This enumeration is used internally to define the possible types of
     * options and store them for use by the parser.
     */
    enum ArgType
    {
        Positional,
        Switch,
        Value
    };

    class Container{
        public:
            const char* getName();
            const char* getPlaceholderText();
            const char* getDescription();

            const ArgType getType();
            
            std::vector<std::string> getIdentifiers();

            // To-Do: Make C++17 version with std::optional support.
            std::pair<bool, std::string> getValue();
            std::pair<bool, std::vector<std::string>> getValues();
            std::pair<bool, std::string> getValidation();

        private:
            struct ContainerImpl;
            ContainerImpl* container_impl;
            
            friend class Parser;

            Container(ContainerImpl* container);
            ~Container();

            bool hasValidator();
            Validator getValidator();
            bool hasCallback();
            Callback getCallback();
    };
}