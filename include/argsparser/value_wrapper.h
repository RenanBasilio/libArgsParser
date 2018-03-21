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
#include <common.h>

namespace ArgsParser
{
    struct ValueWrapper
    {
        const std::vector<std::string> user_input;
        const bool active;

        operator bool() const noexcept;
        
        operator std::string() const noexcept;

        std::string operator[](size_t position) const noexcept;

        size_t size() const noexcept;
    };
}