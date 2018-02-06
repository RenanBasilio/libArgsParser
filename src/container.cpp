/**
 * container.h
 * 
 * This file contains the implementation of methods in the Container class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/containerImpl.h>

namespace ArgsParser
{
    Container::Container(
        ContainerImpl* container
    ) : container_impl(container){};

    Container::~Container(){
        delete container_impl;
        container_impl = nullptr;
    }

    const char* Container::getName(){
        return container_impl->name;
    }

    const ArgType Container::getType(){
        return container_impl->option_type;
    }

    std::vector<std::string> Container::getIdentifiers(){
        return container_impl->identifiers;
    }

    const char* Container::getPlaceholderText(){
        return container_impl->placeholder;
    }

    const char* Container::getDescription(){
        return container_impl->desc;
    }

    std::pair<bool, std::string> Container::getValue(){
        return std::make_pair(container_impl->isActive, container_impl->value);
    }

    std::pair<bool, std::string> Container::getValidation(){
        return std::make_pair(container_impl->validation_succeeded, container_impl->validation_failure_reason);
    }

    Validator Container::getValidator(){
        return container_impl->validator;
    }

    Postprocessor Container::getPostprocessor(){
        return container_impl->postprocessor;
    }

    Callback Container::getCallback(){
        return container_impl->callback;
    }

    bool Container::hasValidator(){
        return (container_impl->validator != nullptr);
    }

    bool Container::hasPostprocessor(){
        return (container_impl->postprocessor != nullptr);
    }

    bool Container::hasCallback(){
        return (container_impl->callback != nullptr);
    }
}