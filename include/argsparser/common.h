/**
 * common.h
 * 
 * This file contains common definitions and declarations used throughout the library.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <string>

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
 * Callback functions must always take a string (the input that will be fed into it)
 * and may return a pointer to anything. The function will be called after validation
 * and the resulting pointer can be retrieved by calling getPostValue() on the container
 * or through the equivalent interface on the parser itself.
 */
typedef void* (*Callback) (const std::string *);