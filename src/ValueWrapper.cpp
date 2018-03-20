/**
 * ValueWrapper.cpp
 * 
 * This file contains the definition of the methods declared in the ValueWrapper
 * class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/valuewrapper.h>

namespace ArgsParser
{
    ValueWrapper::operator bool() const noexcept {
        return active;
    };

    ValueWrapper::operator std::string() const noexcept {
        std::string fullStr = user_input[0];
        for (size_t i = 1; i < user_input.size(); i++)
        {
            fullStr += user_input[i];
        }
        return fullStr;
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
}