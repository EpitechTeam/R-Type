
/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        server.hpp
*/

#ifndef PROJECT_SERVER_HPP
#define PROJECT_SERVER_HPP

#include "Session.hpp"
#include "Parser.hpp"

class Server {
public:
    Server(boost::asio::io_context &io_context,
           const tcp::endpoint &endpoint, const udp::endpoint& udpEndpoint);

    void deliver(const Message &msg, participant_ptr participant);

    udp::endpoint &getUdpEndpoint();
    boost::asio::io_context &getIo_context();

    std::list<Room> _rooms;
private:
    void doAccept();


    tcp::acceptor _acceptor;
    Parser *_parser;
    boost::asio::io_context io_context;

private:
    udp::endpoint _udpEndPoint;
};

#endif //PROJECT_Server_HPP
