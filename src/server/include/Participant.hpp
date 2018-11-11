//
// Created by kahoul on 11/11/18.
//

#ifndef R_TYPE_PARTICIPANT_HPP
#define R_TYPE_PARTICIPANT_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

#include "Message.hpp"

class Participant
{
public:
    virtual ~Participant() {}
    virtual void deliver(const Message& msg) = 0;


    void setName(std::string &name) {
        this->_name = name;
    }

    std::string getName(void) const {
        return this->_name;
    }

private:
    std::string _name;
};

typedef std::shared_ptr<Participant> participant_ptr;


#endif //R_TYPE_PARTICIPANT_HPP
