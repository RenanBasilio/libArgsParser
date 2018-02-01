/**
 * argsparser.h
 * 
 * This file contains the class declaration for the base ArgsParser class without any
 * additional feature modules.
 * 
 * Copyright (C) 2018 Renan Basilio. All rights reserved.
 */

#include <unordered_map>
#include <vector>
#include <string>

class ArgsParser{
    public:
        ArgsParser();
        ~ArgsParser();

        /**
         * This method registers an input parameter to the class. Usage example:
         *      ArgsParser.register_input('File', 'f', 'file', 'The file to open');
         * 
         * The above line will enable parsing the following lines:
         *      myprogram -f example.txt
         *      myprogram --file example.txt
         * 
         * And generate the following help text:
         *      myprogram -h
         *          ...
         *          -f, --file      The file to open.
         */
        bool register_parameter(
            std::string name, 
            std::string short_identifier = NULL,
            std::string long_identifier = NULL, 
            std::string description = NULL);

    private:
        /**
         * This internal structure stores all information related to a registered argument.
         */
        struct DataContainer;

        /** 
         * This vector stores the names of all declared argument types for generating help text.
         */
        std::vector<std::string> names;

        /** 
         * This map stores key value pairs for quick retrieval of arguments by their identifier or name through
         *      get('Help')
         *      get('h')
         * Upon registration of an argument type, both it's identifier and name will be registered as keys pointing
         * to the DataContainer which contains the remainder of the data.
         */
        std::unordered_map<std::string, DataContainer*> dictionary;
}