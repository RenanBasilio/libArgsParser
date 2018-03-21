/**
 * util.h
 * 
 * This file contains the declaration of utility methods in the ArgsParser
 * namespace.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <string>
#include <iostream>

namespace ArgsTools
{
    /**
     * This method checks if an identifier is valid. That is, if the identifier
     * is prefixed by the correct amount of dashes and does not contain any
     * invalid characters. Valid characters are:
     *      "-AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
     * 
     * @param {std::string} identifier The identifier to check.
     * @return {bool} Whether the string is a valid identifier.
     */
    bool check_identifier(const std::string& identifier);

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
    std::string make_identifier(const std::string& string);

    /**
     * This method is an error handler that simply prints the e.what() of an
     * exception object.
     * 
     * @param {std::exception&} e The exception to print.
     */
    void print_error(const std::exception& e);
}