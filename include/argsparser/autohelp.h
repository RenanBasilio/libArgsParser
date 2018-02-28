/**
 * autohelp.h
 * 
 * This file contains the declaration of methods used to automatically
 * generate usage help text.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <iostream>
#include <argsparser/container.h>
#include <argsparser/parser.h>

namespace ArgsParser
{
    /**
     * This method outputs usage help text to the provided stream (which is
     * the standard output stream "std::cout" by default) in the following
     * form:
     *      usage: ${program_name} [<positional>] ... [<args>]
     *          -s, --switch                        ${switch_description}
     *          ...
     * 
     *      options
     *          -o, --option                        ${option_description}
     *          -p, --value-option <placeholder>    ${value_option_description}
     *          ...
     */
    void autohelper(const Parser& parser, std::ostream& stream = std::cout);
}