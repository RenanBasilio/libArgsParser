/**
 * token.h
 * 
 * This file contains the declaration of the Token class and related global
 * defines.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once
#include <argsparser/common.h>

namespace ArgsParser
{
    /**
     * This struct is the type of the id tokens returned by registration methods
     * in the parser. These can be used to more quickly retrieve registered items 
     * by skipping the hashing and lookup processes associated with stl maps.
     * 
     * Objects of the token class are implicitly convertible to bool. Furthermore,
     * the logical equality and comparison operators are implemented to compare
     * objects of this type.
     */
    struct Token
    {
        ArgType type;
        unsigned short position;

        operator bool() const noexcept;

        bool operator==(const Token& other) const noexcept;

        Token operator||(const Token& other) const noexcept;
    };

    const Token NULL_TOKEN = {ArgType::Null, 0};
}