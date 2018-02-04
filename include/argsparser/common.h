/**
 * common.h
 * 
 * This file contains common definitions and declarations used throughout the library.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <string>

namespace ArgsParser
{
    /**
     * This is the declaration of a validator function.
     * 
     * Validator functions must always take a string (the input that will be fed into it)
     * and return a boolean of whether the validation succeeded or not. 
     * The result of the validation process can be retrieved by calling getValidation() on
     * the container or through the equivalent interface on the parser itself.
     * 
     * Optionally, validator functions may throw an exception with the error that caused
     * validation to fail. The exception will be handled internally and the error message
     * made available if validation failures are not to be considered critical.
     */
    typedef bool (*Validator) (const std::string *);

    /**
     * This is the declaration of a callback or post-processor function.
     * 
     * Callback functions must always take a string (the input that will be fed into it),
     * while post-processors may take an additional pointer to anything (be it an array
     * of additional parameters or simply a position where it will place a result).
     * This function will be called after validation completes if it does so successfully.
     */
    typedef void (*Callback) (const std::string *);
    typedef void (*Postprocessor) (const std::string *, const void *);    
}