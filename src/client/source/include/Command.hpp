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

#include "Message.hpp"

class Command {
public:
    Command(std::string command)
        : _str(command)
    {
        this->init(command);
    }

    Command(Message &msg) {

        char *cstr = new char [msg.body_length() + 1];
        strncpy (cstr, msg.body(), msg.body_length());

        this->_str = std::string(cstr);
        std::cout << "STR: " << this->_str << std::endl;
        this->init(this->_str);
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

    std::string toStr() const {
        return std::string(this->_str);
    }

private:
    void init(std::string command) {

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

private:
    std::string _command;
    std::vector <std::string> _args;
    std::string _str;
};

#endif //R_TYPE_COMMAND_HPP
