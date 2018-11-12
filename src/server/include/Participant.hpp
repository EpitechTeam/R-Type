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

class Participant
{
public:
    virtual ~Participant() {}
    virtual void deliver(const Message& msg) = 0;


    void setName(std::string &name);

    std::string getName(void) const;

    void setRoom(Room *room);

    virtual void exitRoom(void) = 0;

    Room *_currentRoom = NULL;

private:
    std::string _name;
};

typedef std::shared_ptr<Participant> participant_ptr;


#endif //R_TYPE_PARTICIPANT_HPP
