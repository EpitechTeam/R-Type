//
// Created by kahoul on 11/11/18.
//

#ifndef R_TYPE_COMMAND_HPP
#define R_TYPE_COMMAND_HPP

#include <iostream>
#include <list>
#include <vector>
#include <stdio.h>
#include <string.h>

class Command {
public:
    Command(std::string command) {
        char *cstr = new char [command.length() + 1];
        strcpy (cstr, command.c_str());

        char *tmp = strtok(cstr, " ");
        bool first = true;
        while (tmp) {
            if (first) {
                this->_command = tmp;
            } else {
                this->_args.emplace_back(tmp);
            }
            tmp = strtok(NULL, " ");
            first = false;
        }
    }

    std::string getCommand(void) const {
        return this->_command;
    }

    std::string getArg(int pos = 0) {
        return (this->_args.at(pos));
    }

    std::size_t argLen(void) const {
        return this->_args.size();
    }

private:
    std::string _command;
    std::vector <std::string> _args;
};

#endif //R_TYPE_COMMAND_HPP
