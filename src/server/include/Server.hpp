
/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        server.hpp
*/

#ifndef PROJECT_SERVER_HPP
#define PROJECT_SERVER_HPP

#include "Session.hpp"

class Server {
public:
    Server(boost::asio::io_context &io_context,
           const tcp::endpoint &endpoint);

    void deliver(const Message &msg);

private:
    void doAccept();


    tcp::acceptor _acceptor;
    Room _room;
};

#endif //PROJECT_Server_HPP
