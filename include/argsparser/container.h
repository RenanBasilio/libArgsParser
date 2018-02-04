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
            const char  getShortId();
            const char* getLongId();
            const char* getPlaceholderText();
            const char* getDescription();

            // To-Do: Make C++17 version with std::optional support.
            std::pair<bool, std::string> getValue();
            std::pair<bool, std::string> getValidation();

            Postprocessor getPostprocessor();
        private:
            struct ContainerImpl;
            ContainerImpl* container_impl;
            
            friend class Parser;
            Container(
                std::string name, 
                char short_id, 
                std::string long_id, 
                std::string placeholder, 
                std::string desc,
                Validator validator,
                bool validation_critical,
                Postprocessor callback);
    };
}