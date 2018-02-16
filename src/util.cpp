/**
 * util.cpp
 * 
 * This file contains the implementation of utility methods in the ArgsParser
 * namespace.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include<argsparser/util.h>

namespace ArgsParser
{
    std::string make_identifier(std::string string){

        // First check if this is a valid string.
        const size_t invalid_char = string.find_first_not_of("-AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
        if(invalid_char != std::string::npos) {
            throw std::runtime_error( std::string("Invalid character '") + string.at(invalid_char) + std::string("'") );
        }
        // Then check if the string contains at least one letter (and does not end in a dash)
        if(string.length() > 0 && string.at(string.length()-1) == '-'){
            throw std::runtime_error( "Identifier must not end in a dash.");
        }

        // Next, check if string is correctly prefixed. If not, add prefix dashes.
        // If string is 1 character long, prefix a dash to it.
        if (string.length() == 1) string.insert(string.begin(), '-');
        // Otherwise, get how many prefix '-' characters it has and add enough to make two.
        else {
            const size_t first_char = string.find_first_not_of("-");

            // If first character is the third character in the string, return.
            // Otherwise, fix the prefix.
            if( (string.length() != 2 && string.at(0) != '-') || first_char != 2)
            {
                std::string identifier = string.substr(first_char);
                string = identifier.length() == 1? ("-" + identifier) : ("--" + identifier);
            }
        }

        return string;
    };   
}