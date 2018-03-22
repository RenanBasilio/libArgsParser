/**
 * typed_value_wrapper.h
 * 
 * This file contains the declaration of the TypedValueWrapper struct, a template
 * utility that is implicitly convertible to bool or a user defined type used for
 * returning typed argument values.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once
#include <argsparser/common.h>

namespace ArgsParser
{
    template <typename T>
    struct TypedValueWrapper
    {
        const std::vector<T> typed_input;
        const bool active;

        operator bool() const noexcept {
            return active;
        };

        operator T() const noexcept {
            if (typed_input.size() > 0) return typed_input[0];
            else return T();
        };

        operator std::vector<T>() const noexcept {
            return typed_input;
        };

        T operator[](size_t position) const noexcept {
            // Return the requested value if the position specified is valid
            if (position < typed_input.size()) return typed_input[position];
            
            // Return the last value if the position specified is after the end of the vector.
            else if (typed_input.size() != 0) return typed_input[typed_input.size()-1];
            
            // Return an empty object of type T otherwise.
            else return T();
        }

        size_t size() const noexcept {
            return typed_input.size();
        };
    };
}