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
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const Callback& callback
        ) :
        name_(name),
        identifiers_(identifiers),
        description_(description),
        callback_(callback),
        active_(false) { };

    Container::~Container(){

    }

    Container* Container::clone() const{
        Container* copy = new Container(
            name_,
            identifiers_,
            description_,
            callback_
        );
        copy->active_ = active_;
        return copy;
    }

    UserInputContainer::UserInputContainer(
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const std::string& placeholder_text,
        const Validator& validator,
        const Callback& error_callback,
        const Callback& callback
    ) : Container(name, identifiers, description, callback),
        placeholder_text_(placeholder_text),
        validator_(validator),
        error_callback_(error_callback),
        validation_(false),
        validation_failure_reason_("No input value.")
    { };

    UserInputContainer::~UserInputContainer(){

    }

    UserInputContainer* UserInputContainer::clone() const{
        UserInputContainer* copy = new UserInputContainer(
            name_,
            identifiers_,
            description_,
            placeholder_text_,
            validator_,
            error_callback_,
            callback_
        );
        copy->user_input_ = user_input_;
        copy->validation_ = validation_;
        copy->validation_failure_reason_ = validation_failure_reason_;
        return copy;
    }

    const std::string Container::getName(){
        return name_;
    };

    const std::string Container::getDescription(){
        return description_;
    };

    const std::vector<std::string> Container::getIdentifiers(){
        return identifiers_;
    };

    const bool Container::isActive(){
        return active_;
    };

    const std::string UserInputContainer::getPlaceholderText(){
        return placeholder_text_;
    };

    const std::string UserInputContainer::getUserInput(){
        return user_input_;
    };

    const std::pair<bool, std::string> UserInputContainer::getValidation(){
        return std::make_pair(validation_, validation_failure_reason_);
    };

    template<>
    const std::string TypedInputContainer<std::string>::getValue() {
        if (converter_ != nullptr) return converted_value_;
        else return user_input_;
    };
}