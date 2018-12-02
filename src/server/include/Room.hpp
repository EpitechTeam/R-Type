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
#include <boost/bind.hpp>
#include <boost/array.hpp>


#include "Message.hpp"
#include "Participant.hpp"
#include "UDPGame.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

//----------------------------------------------------------------------

typedef std::deque<Message> MessageQueue;

//----------------------------------------------------------------------

class Room {
public:
    Room(boost::asio::io_context &, std::string &name, int maxSlots, const udp::endpoint&);

    static Room *find(std::list<Room> &rooms, std::string &name) {

        for (auto &i : rooms) {
            if (i.getName() == name) {
                return &i;
            }
        }
        return (NULL);
    }

    std::string getName() const {
        return this->_name;
    }

    void join(participant_ptr participant);

    void leave(participant_ptr participant);

    void deliver(const Message &msg);

    bool isAllPlayerReady() const ;

    std::string getMessages();

    std::set <participant_ptr> _participants;

public:
    enum {
        max_recent_msgs = 9
    };
    MessageQueue _recent_msgs;
    std::string _name;
    int _maxSlots;
    UDPGame _game;
    clock_t _timeout = 0;
};


#endif //PROJECT_ROOM_HPP
