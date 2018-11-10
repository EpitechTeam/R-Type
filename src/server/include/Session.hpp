//
// Created by kahoul on 10/11/18.
//

#ifndef PROJECT_SESSION_HPP
#define PROJECT_SESSION_HPP

#include "Room.hpp"

class Server;

class Session
        : public Participant,
          public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Server *server);

    void start();

    void deliver(const Message &msg);

private:
    void doReadHeader();

    void doReadBody();

    void doWrite();

    Server *_server;
    tcp::socket _socket;
    Message _read_msg;
    MessageQueue _write_msgs;
};

#endif //PROJECT_SESSION_HPP