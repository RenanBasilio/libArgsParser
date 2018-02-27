/**
 * common.h
 * 
 * This file contains common definitions and declarations used throughout the library.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <exception>
#include <functional>

namespace ArgsParser
{
    /**
     * This defines the type of the id tokens returned by registration methods
     * in the parser. These can be used to more quickly retrieve registered
     * itemby skipping the hashing and lookup processes associated with stl
     * maps.
     * 
     * A token of 00000000 is referred to as a null token and represents an
     * invalid or otherwise unsuccessful registration.
     */
    typedef unsigned short int token;
    const unsigned short int null_token = 0;

    /**
     * This is the declaration of a validator function.
     * 
     * Validator functions must always take a string (the input that will be 
     * fed into it) and return a boolean of whether the validation succeeded 
     * or not. 
     * 
     * The result of the validation process can be retrieved by calling 
     * getValidation() on the container or through the equivalent interface 
     * on the parser itself.
     * 
     * Optionally, validator functions may throw an exception with the error 
     * that caused validation to fail. The exception will be handled internally
     * and the error message made available if validation failures are not to 
     * be considered critical. Otherwise it will be rethrown.
     * 
     * Some sample validator methods are defined in samples/validators.h
     */
    using Validator = std::function<bool(const std::string&)>;

    /**
     * This is the declaration of a callback function. 
     * 
     * Callback methods do not take any arguments and must not return anything. 
     * They are called after validation but do not require the value to validate
     * successfully. This type of method is recommended to be called as an entry 
     * point for a positional argument or switch.
     * 
     * Some sample callback methods are defined in samples/callbacks.h
     */
    using Callback = std::function<void()>;

    /**
     * This is the declaration of a converter function.
     * 
     * Converter functions are used to convert input from user submitted strings
     * to another type. As such, they must always take a string and a pointer.
     * The string will be the user input string, and the pointer will be an
     * argument of the template instantiated type to contain the result.
     */
    template <typename T> using Converter = std::function<T(const std::string&)>;
}