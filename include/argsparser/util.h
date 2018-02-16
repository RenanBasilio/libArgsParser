/**
 * util.h
 * 
 * This file contains the declaration of utility methods in the ArgsParser
 * namespace.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/common.h>

namespace ArgsParser
{
    /**
     * This method makes a valid identifier from a string. This is done by
     * prefixing a single '-' character to the string if it is only one
     * character long and '--' if it contains multiple character.
     * 
     * This method throws an exception if the string contains invalid
     * characters.
     * 
     * @param {std::string} string The string to make a valid identifier from.
     * @return {std::string} A valid identifier string.
     * @except {std::runtime_error} String contains invalid characters.
     */
    std::string make_identifier(std::string string);
}