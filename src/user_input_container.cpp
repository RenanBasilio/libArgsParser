/**
 * UserInputContainer.cpp
 * 
 * This file contains the definition of methods in the UserInputContainer class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/container.h>

namespace ArgsParser
{
    UserInputContainer::UserInputContainer(
        const ArgType type,
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const std::string& placeholder_text,
        const size_t max_values,
        const Validator& validator,
        const ErrorHandler& error_callback,
        const Callback& callback
    ) : Container(type, name, identifiers, description, callback),
        placeholder_text_(placeholder_text),
        max_values_(max_values),
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
            max_values_,
            validator_,
            error_callback_,
            callback_
        );
        copy->user_input_ = user_input_;
        copy->validation_ = validation_;
        copy->validation_failure_reason_ = validation_failure_reason_;
        return copy;
    }

    std::string UserInputContainer::getPlaceholderText() const{
        return placeholder_text_;
    };

    std::vector<std::string> UserInputContainer::getUserInput() const{
        return user_input_;
    };

    void UserInputContainer::setActive(const std::string& input){
        if(!active_) Container::setActive();
        user_input_.push_back(input);
    }

    size_t UserInputContainer::getInputSize(){
        return user_input_.size();
    }

    size_t UserInputContainer::getMaxInputs(){
        return max_values_;
    }

    ValueWrapper UserInputContainer::getValue() const{
        return {user_input_, active_};
    }

    std::pair<bool, std::string> UserInputContainer::getValidation() const{
        return std::make_pair(validation_, validation_failure_reason_);
    };
}