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

        private:
            const ContainerImpl* container_impl;

            friend class Parser;

            std::tuple<bool, Callback, Postprocessor> getPostFunction();
    };
}