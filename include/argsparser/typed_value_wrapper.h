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
        const std::vector<T> typed_input_;
        const bool active_;

        operator bool() const noexcept;

        operator T() const noexcept;

        operator std::vector<T>() const noexcept;

        T operator[](size_t position) const noexcept;

        size_t size() const noexcept;

        TypedValueWrapper(bool active, std::vector<T> typed_input);
    };


    /////////////////////// Template Method Definitions ///////////////////////

    template <typename T>
    TypedValueWrapper<T>::operator bool() const noexcept {
        return active_;
    };

    template <typename T>
    TypedValueWrapper<T>::operator T() const noexcept {
        if (typed_input_.size() > 0) return typed_input_[0];
        else return T();
    };

    template <typename T>
    TypedValueWrapper<T>::operator std::vector<T>() const noexcept {
        return typed_input_;
    };
    
    template <typename T>
    T TypedValueWrapper<T>::operator[](size_t position) const noexcept {
        // Return the requested value if the position specified is valid
        if (position < typed_input_.size()) return typed_input_[position];
        
        // Return the last value if the position specified is after the end of the vector.
        else if (typed_input_.size() != 0) return typed_input_[typed_input_.size()-1];
        
        // Return an empty object of type T otherwise.
        else return T();
    };

    template <typename T>
    size_t TypedValueWrapper<T>::size() const noexcept {
        return typed_input_.size();
    };

    template <typename T>
    TypedValueWrapper<T>::TypedValueWrapper( bool active, std::vector<T> typed_input):
        active_(active),
        typed_input_(typed_input) {};
}