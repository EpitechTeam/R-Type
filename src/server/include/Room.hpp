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
#include "Participant.hpp"

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<Message> MessageQueue;

//----------------------------------------------------------------------

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
