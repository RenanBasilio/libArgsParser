/**
 * container.h
 * 
 * This file contains the public interface of the container class used to store
 * data.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#pragma once

#include <algorithm>
#include <argsparser/value_wrapper.h>
#include <argsparser/common.h>

namespace ArgsParser
{
    /**
     * This template base class is used to store basic information about an
     * argument type to be parsed.
     * This class is used when a switch is registered, as it has a smaller
     * memory footprint.
     */
    class Container{
        public:
            /**
             * This method retrieves the name assigned to this container.
             * 
             * @return {string} The name of the argument stored in this container.
             */
            std::string getName() const noexcept;

            /**
             * This method retrieves the type of this container.
             * 
             * @return {ArgType} The type of the argument stored in this container.
             */
            ArgType getType() const noexcept;

            /**
             * This method retrieves the description assigned to this container.
             * 
             * @return {string} The description of the argument stored in this container.
             */
            std::string getDescription() const noexcept;
            
            /**
             * This method retrives the list of identifiers registered to this
             * container.
             * 
             * @return {vector<string>} The list of identifiers associated with this container.
             */
            std::vector<std::string> getIdentifiers() const noexcept;

            /**
             * This method retrieves whether this container is active. This is
             * always false before parsing, and will be set if this container
             * refers to an option called by the user.
             * 
             * @return {bool} Whether the parser found this container in the command line.
             */
            bool isActive() const noexcept;

            /**
             * Constructor of the container class.
             * 
             * @param {ArgType} type The type of the argument.
             * @param {string} name The name of the argument.
             * @param {vector<string>} identifiers The list of identifiers associated with the argument.
             * @param {string} description The description of the argument.
             * @param {Callback} callback The method to call during post processing if this argument is found by the parser.
             */
            Container(
                const ArgType type,
                const std::string& name,
                const std::vector<std::string>& identifiers,
                const std::string& description,
                const Callback& callback
            );

            /**
             * Destructor of the container class.
             */
            virtual ~Container(); 

            /**
             * Cloning method.
             * 
             * @return {Container*} A pointer to a new Container object cloned from this object.
             */
            virtual Container* clone() const;

        protected:
            const std::string name_;
            const std::string description_;
            const std::vector<std::string> identifiers_;
            const Callback callback_;
            const ArgType type_;

            bool active_;

            friend class Parser;

            /**
             * This method sets the container to active. 
             * It is private, and can only be called by the parser. It will be
             * called if the parser finds this argument in the command line.
             */
            void setActive();

            /**
             * This method executes all post-processing logic associated with 
             * this container.
             * It is virtual, as the derived classes use different post-processing
             * logic to account for features such as validation or conversion. 
             */
            virtual void postProcess();

    };
}