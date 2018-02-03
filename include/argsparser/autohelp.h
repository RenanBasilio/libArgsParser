/**
 * autohelpe.h
 * 
 * This file contains the declaration of methods used to automatically
 * generate usage help text.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <iostream>
#include <argsparser/container.h>

namespace ArgsParser
{
    /**
     * This method outputs usage help text to the provided stream (which is
     * the standard output stream "std::cout" by default) in the following
     * form:
     *      usage: ${program_name} [<${parameter1}>] ... [<options>]
     *          -s, --switch                        ${switch_description}
     *          ...
     * 
     *      options
     *          -o, --option                        ${option_description}
     *          -p, --parameter-opt <placeholder>   ${parameter_option_description}
     *          ...
     */
    void autohelper(std::ostream& stream = std::cout);
}