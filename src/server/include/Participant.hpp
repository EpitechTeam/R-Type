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

class Room;

enum Type { UNKNOWN , PreGameObj, GameObj };

class Participant
{
public:
    Participant()
        : _type(UNKNOWN)
    {}
    virtual ~Participant() {}
    virtual void deliver(const Message& msg) = 0;


    void setName(std::string &name);

    std::string getName(void) const;

    void setRoom(Room *room);

    Type getType(void) const;

    void setReady() {

        if (_currentRoom) {
            this->_ready = !this->_ready;
        }
    }

    bool getReady() const {
        return this->_ready;
    }

    Room *_currentRoom = NULL;
    Type _type;

private:
    std::string _name;
    bool _ready = false;

};

typedef std::shared_ptr<Participant> participant_ptr;


#endif //R_TYPE_PARTICIPANT_HPP
