/**
 * container.h
 * 
 * This file contains the implementation of methods in the Container class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/container.h>

namespace ArgsParser
{
    Container::Container(
        const ArgType type,
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const Callback& callback
        ) :
        type_(type),
        name_(name),
        identifiers_(identifiers),
        description_(description),
        callback_(callback),
        active_(false) { };

    Container::~Container(){

    }

    Container* Container::clone() const{
        Container* copy = new Container(
            type_,
            name_,
            identifiers_,
            description_,
            callback_
        );
        copy->active_ = active_;
        return copy;
    }

    std::string Container::getName() const{
        return name_;
    };

    ArgType Container::getType() const{
        return type_;
    }

    std::string Container::getDescription() const{
        return description_;
    };

    std::vector<std::string> Container::getIdentifiers() const{
        return identifiers_;
    };

    bool Container::isActive() const{
        return active_;
    };

    void Container::setActive(){
        active_ = true;
    };

    void Container::postProcess(){
        if(callback_ != nullptr) callback_();
    };
}