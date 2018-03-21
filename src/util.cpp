/**
 * util.cpp
 * 
 * This file contains the implementation of utility methods in the ArgsParser
 * namespace.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include<argsparser/util.h>

namespace ArgsTools
{
    bool check_identifier(const std::string& string){

        // First check if this is a valid string.
        const size_t invalid_char = string.find_first_not_of("-AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
        if(invalid_char != std::string::npos) {
            throw std::invalid_argument( std::string("Invalid character '") + string.at(invalid_char) + std::string("'") );
        }
        // Then check if the string contains at least one letter (and does not end in a dash)
        if(string.length() > 0 && string.at(string.length()-1) == '-'){
            throw std::invalid_argument( "Identifier must not end in a dash.");
        }

        // Next, check if string is correctly prefixed.
        if(string.length() == 2 && string.at(0) == '-') return true;
        size_t first_char = string.find_first_not_of('-');
        if(string.length()-first_char > 1 && first_char == 2) return true;

        // If neither check passes, return false.
        return false;
    }

    std::string make_identifier(const std::string& string){

        if (check_identifier(string)) return string;

        size_t first_char = string.find_first_not_of('-');

        // If the string is valid but not correctly prefixed, remove any 
        // prefixing dashes and prefix with the correct amount.
        std::string ret_string = string;
        ret_string.replace(
            0,
            first_char,
            (string.length()-first_char == 1? "-":"--")
        );
        
        return ret_string;
    };

    void print_error(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    };
}