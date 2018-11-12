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


#include "Message.hpp"
#include "Participant.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

//----------------------------------------------------------------------

typedef std::deque<Message> MessageQueue;

//----------------------------------------------------------------------

class UDPServer {
public:
    UDPServer(boost::asio::io_context &, const udp::endpoint &);

    ~UDPServer();

private:
    void startReceive();

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_transferred);

    void handleSend(std::shared_ptr<std::string> message,
                    const boost::system::error_code& ec,
                    std::size_t bytes_transferred);

private:
    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    std::array<char, 1024> _recvBuffer;

};
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
    }

    bool containParticipant(participant_ptr participant) {

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
    UDPServer _udpServer;

};


#endif //PROJECT_ROOM_HPP
