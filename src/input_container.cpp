/**
 * InputContainer.cpp
 * 
 * This file contains the definition of methods in the InputContainer class.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <argsparser/input_container.h>

namespace ArgsParser
{
    InputContainer::InputContainer(
        const ArgType type,
        const std::string& name,
        const std::vector<std::string>& identifiers,
        const std::string& description,
        const std::string& placeholder_text,
        const size_t max_values,
        const Validator<std::string>& validator,
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

    InputContainer::~InputContainer(){

    }

    InputContainer* InputContainer::clone() const{
        InputContainer* copy = new InputContainer(
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

    std::string InputContainer::getPlaceholderText() const noexcept{
        return placeholder_text_;
    };

    std::vector<std::string> InputContainer::getUserInput() const noexcept{
        return user_input_;
    };

    size_t InputContainer::getInputSize() const noexcept{
        return user_input_.size();
    };

    size_t InputContainer::getMaxInputs() const noexcept{
        return max_values_;
    };

    ValueWrapper InputContainer::getValue() const noexcept{
        return {user_input_, active_};
    };

    std::pair<bool, std::string> InputContainer::getValidation() const noexcept{
        return std::make_pair(validation_, validation_failure_reason_);
    };

    void InputContainer::setActive(const std::string& input){
        if(!active_) Container::setActive();
        user_input_.push_back(input);
    };

    void InputContainer::postProcess(){
        if (validator_ != nullptr) {
            for (size_t i = 0; i < user_input_.size(); i++) {
                try{
                    bool valid = validator_(user_input_[i]);
                    if (!valid) throw std::runtime_error("Unspecified validation error.");
                }
                catch (std::exception& e) {
                    validation_ = false;
                    validation_failure_reason_ = e.what();
                    
                    if (error_callback_ != nullptr) error_callback_(e);
                    return;
                }
            }
        }
        if (callback_ != nullptr) callback_();

    };
}