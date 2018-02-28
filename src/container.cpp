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

    UserInputContainer::UserInputContainer(
        const ArgType type,
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const std::string& placeholder_text,
        const Validator& validator,
        const Callback& error_callback,
        const Callback& callback
    ) : Container(type, name, identifiers, description, callback),
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
            type_,
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

    std::string UserInputContainer::getPlaceholderText() const{
        return placeholder_text_;
    };

    std::string UserInputContainer::getUserInput() const{
        return user_input_;
    };

    std::pair<bool, std::string> UserInputContainer::getValidation() const{
        return std::make_pair(validation_, validation_failure_reason_);
    };

    template<>
    std::string TypedInputContainer<std::string>::getValue() const{
        if (converter_ != nullptr) return converted_value_;
        else return user_input_;
    };
}