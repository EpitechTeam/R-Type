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

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

//----------------------------------------------------------------------

typedef std::deque<Message> MessageQueue;

//----------------------------------------------------------------------

struct TestPlayer {
    int id;
    std::string name;
    int posX;
    int posY;
};

typedef std::string (*FncPtr)(std::vector<TestPlayer>);



class UDPParser {
public:
    UDPParser();
    ~UDPParser();

    void parseCommand(const std::string &, std::vector<TestPlayer>);

    std::string getCmdToSend();

private:
    static std::string getAllPlayerPositions(std::vector<TestPlayer>);

private:
    std::string _cmdToSend;
    std::map<std::string, FncPtr> _playerFncs;

};

//----------------------------------------------------------------------


class UDPServer {
public:
    UDPServer(boost::asio::io_context &, const udp::endpoint &);

    ~UDPServer();

private:
    void startReceive();

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_transferred);

    void handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& error,
                    std::size_t bytes_transferred);

private:
    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    boost::array<char, 1024> _recvBuffer;
    UDPParser _udpParser;
    std::vector<TestPlayer> _players;

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
        return (NULL);
    }

    std::string getName() const {
        return this->_name;
    }

    void join(participant_ptr participant);

    void leave(participant_ptr participant);

    void deliver(const Message &msg);

    std::string getMessages();

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
