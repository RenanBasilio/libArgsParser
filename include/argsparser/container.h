/**
 * container.h
 * 
 * This file contains the declarations of structures used to store data for the
 * parser.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <argsparser/containerImpl.h>
#include <argsparser/common.h>

namespace ArgsParser
{
    class Container{
        public:
            char* getName();
            char  getShortId();
            char* getLongId();
            char* getPlaceholderText();
            char* getDescription();

            // To-Do: Make C++17 version with std::optional support.
            std::pair<bool, std::string> getValue();
            std::pair<bool, std::string> getValidation();
            std::pair<bool, void*>       getPostResult();

        private:
            const ContainerImpl* container_impl;

            friend class Parser;

            /**
             * Constructor for the Container structure.
             */
            Container(std::string name, 
                char short_id, 
                std::string long_id, 
                std::string placeholder, 
                std::string desc,
                Validator validator,
                bool validation_critical,
                Callback callback)
                {

                }
    };
}