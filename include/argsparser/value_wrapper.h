/**
 * value_wrapper.h
 * 
 * This file contains the declaration of the ValueWrapper struct, a  utility
 * class that is implicitly convertible to bool or string for returning
 * argument values.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once
#include <argsparser/common.h>

namespace ArgsParser
{
    struct ValueWrapper
    {
        const std::vector<std::string> user_input;
        const bool active;

        // Implicit conversion to bool.
        operator bool() const noexcept;
        
        // Implicit conversion to string.
        operator std::string() const noexcept;

        // Implicit conversion to c string.
        operator const char*() const noexcept;
        
        // Implicit conversion to vector of strings.
        operator std::vector<std::string>() const noexcept;

        // Accessor operator.
        std::string operator[](size_t position) const noexcept;

        // Number of values in this wrapper.
        size_t size() const noexcept;

        // Explicit conversion to string.
        std::string to_string() const noexcept;
    };
}