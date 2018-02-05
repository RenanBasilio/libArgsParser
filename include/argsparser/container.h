/**
 * container.h
 * 
 * This file contains the declarations of structures used to store data for the
 * parser.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <argsparser/common.h>

namespace ArgsParser
{
    class Container{
        public:
            const char* getName();
            const char* getPlaceholderText();
            const char* getDescription();
            
            std::vector<std::string> getIdentifiers();

            // To-Do: Make C++17 version with std::optional support.
            std::pair<bool, std::string> getValue();
            std::pair<bool, std::string> getValidation();

        private:
            struct ContainerImpl;
            ContainerImpl* container_impl;
            
            friend class Parser;

            Container(
                std::string name, 
                std::vector<std::string> identifiers,
                std::string placeholder, 
                std::string desc,
                bool validation_critical,
                Validator validator,
                Postprocessor postprocessor,
                Callback callback);

            ~Container();

            bool hasValidator();
            Validator getValidator();
            bool hasPostprocessor();
            Postprocessor getPostprocessor();
            bool hasCallback();
            Callback getCallback();
    };
}