
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
    Server(const int port);

    ~Server();

    void deliver(const Message &msg, participant_ptr participant);

    udp::endpoint &getUdpEndpoint();

    boost::asio::io_context &getIo_context();

    void deleteEmptyRooms();

    void run();

    void RoomAdd(std::string &name, int maxSlots);

    std::list <Room> _rooms;

private:
void doAccept();

boost::asio::io_context io_context;

tcp::acceptor _acceptor;

udp::endpoint _udpEndPoint;

std::mutex roomMutex;

Parser *_parser;
std::thread *_updateT = NULL;
};

#endif //PROJECT_Server_HPP
