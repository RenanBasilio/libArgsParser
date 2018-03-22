/**
 * token.cpp
 * 
 * This file contains the definition of methods used by the Token class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/token.h>

namespace ArgsParser
{
    Token::operator bool() const noexcept {
        return (type != ArgType::Null);
    };

    bool Token::operator==(const Token& other) const noexcept {
        return (other.type == type && other.position == position);
    };

    Token Token::operator||(const Token& other) const noexcept {
        return ( type != ArgType::Null? *this : other );
    };
}