/**
 * autohelp.cpp
 * 
 * This file contains the implementation for auto-help methods included in the
 * Parser class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/autohelp.h>

namespace ArgsParser
{
    void autohelper(const Parser& parser, std::ostream& stream){

        std::vector<Token> tokens = parser.getRegisteredTokens();
        std::vector<std::string> positional_strings;
        std::vector<std::pair<std::string, std::string>> switch_strings;
        std::vector<std::pair<std::string, std::string>> option_strings;
        size_t max_id_size = 0;

        for(Token var : tokens)
        {
            const Container* container = parser.getContainer(var);

            if(container->getType() == ArgType::Positional)
            {
                positional_strings.push_back(container->getPlaceholderText());
            }
            else
            {
                std::string id_string = "";
                std::vector<std::string> identifiers = container->getIdentifiers();
                for(size_t i = 0; i < identifiers.size(); i++){
                    id_string += identifiers[i];
                    if( i != identifiers.size() - 1) id_string += ", ";
                }
                if(container->getType() == ArgType::Switch) 
                    switch_strings.push_back(std::pair<std::string, std::string>(id_string, container->getDescription()));
                else{
                    id_string += " <" + container->getPlaceholderText() + ">";
                    option_strings.push_back(std::pair<std::string, std::string>(id_string, container->getDescription()));
                }
                max_id_size = std::max(max_id_size, id_string.length()+4);
            }
        }
        
        stream << "usage: " << parser.getProgramName() << " ";
        for(size_t i = 0; i < positional_strings.size(); i++)
        {
            stream << "[" << positional_strings[i] << "] ";
        }
        stream << "[options]" << std::endl;
        for(size_t i = 0; i < switch_strings.size(); i++)
        {
            stream << "    "
                   << switch_strings[i].first 
                   << std::string(max_id_size - switch_strings[i].first.size(), ' ')
                   << switch_strings[i].second
                   << std::endl;
        }
        stream << std::endl << "options:" <<std::endl;
        for(size_t i = 0; i < option_strings.size(); i++)
        {
            stream << "    "
                   << option_strings[i].first
                   << std::string(max_id_size - option_strings[i].first.size(), ' ')
                   << option_strings[i].second
                   << std::endl;
        }
    };
}