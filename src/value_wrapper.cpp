/**
 * ValueWrapper.cpp
 * 
 * This file contains the definition of the methods declared in the ValueWrapper
 * class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/value_wrapper.h>

namespace ArgsParser
{
    ValueWrapper::operator bool() const noexcept {
        return active;
    };

    ValueWrapper::operator std::vector<std::string>() const noexcept {
        return user_input;
    };

    ValueWrapper::operator const char *() const noexcept {
        return to_string().c_str();
    };

    std::string ValueWrapper::operator[](size_t position) const noexcept {
        // Return the requested value if the position specified is valid
        if (position < user_input.size()) return user_input[position];
        
        // Return the last value if the position specified is after the end of the vector.
        else if (user_input.size() != 0) return user_input[user_input.size()-1];
        
        // Return an empty string if this container contains no user input.
        else return "";
    };

    size_t ValueWrapper::size() const noexcept {
        return user_input.size();
    }

    std::string ValueWrapper::to_string() const noexcept {
        std::string string;
        for (size_t i = 0; i < size(); i++) {
            string += user_input[i];
            if (i < size() - 1) string += " ";
        }
        return string;
    }
}