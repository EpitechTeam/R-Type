//
// Created by kahoul on 10/11/18.
//

#ifndef PROJECT_ROOM_HPP
#define PROJECT_ROOM_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
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
};

typedef std::shared_ptr<Participant> participant_ptr;


class Room {
public:
    void join(participant_ptr participant);

    void leave(participant_ptr participant);

    void deliver(const Message &msg);

private:
    std::set <participant_ptr> _participants;
    enum {
        max_recent_msgs = 100
    };
    MessageQueue _recent_msgs;
};

#endif //PROJECT_ROOM_HPP
