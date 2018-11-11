//
// Created by kahoul on 10/11/18.
//

#ifndef PROJECT_ROOM_HPP
#define PROJECT_ROOM_HPP

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

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<Message> MessageQueue;

//----------------------------------------------------------------------

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


class Room {
public:
    Room(std::string &name, int maxSlots);

    static Room *find(std::list<Room> &rooms, std::string &name) {

        for (auto &i : rooms) {
            if (i.getName() == name) {
                return &i;
            }
        }
    }

    std::string getName() const {
        return this->_name;
    }

    void join(participant_ptr participant);

    void leave(participant_ptr participant);

    void deliver(const Message &msg);



    std::set <participant_ptr> _participants;
private:
    enum {
        max_recent_msgs = 100
    };
    MessageQueue _recent_msgs;
    std::string _name;
    int _maxSlots;
};

#endif //PROJECT_ROOM_HPP
