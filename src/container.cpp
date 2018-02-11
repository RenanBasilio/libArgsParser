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

    const std::string Container::getName(){
        return std::string(container_impl->name);
    }

    const ArgType Container::getType(){
        return container_impl->option_type;
    }

    const std::vector<std::string> Container::getIdentifiers(){
        return container_impl->identifiers;
    }

    const std::string Container::getPlaceholderText(){
        return std::string(container_impl->placeholder);
    }

    const std::string Container::getDescription(){
        return std::string(container_impl->desc);
    }

    bool Container::isActive(){
        return container_impl->isActive;
    }

    std::pair<bool, std::string> Container::getValue(){
        const bool isActive = container_impl->isActive;
        return std::make_pair(isActive, isActive? container_impl->value[0] : "");
    }

    std::pair<bool, std::vector<std::string>> Container::getValues(){
        return std::make_pair(container_impl->isActive, container_impl->value);
    }

    std::pair<bool, std::string> Container::getValidation(){
        return std::make_pair(container_impl->validation_succeeded, container_impl->validation_failure_reason);
    }

    Validator Container::getValidator(){
        return container_impl->validator;
    }

    Callback Container::getCallback(){
        return container_impl->callback;
    }

    bool Container::hasValidator(){
        return (container_impl->validator != nullptr);
    }

    bool Container::hasCallback(){
        return (container_impl->callback != nullptr);
    }
}