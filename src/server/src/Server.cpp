//
// Created by kahoul on 10/11/18.
//

#include "../include/Server.hpp"

Server::Server
(boost::asio::io_context &io_context, const tcp::endpoint &endpoint, const udp::endpoint& udpEndpoint)
        : _acceptor(io_context, endpoint), _parser(new Parser(this)),
          _udpEndPoint(udpEndpoint) {
    doAccept();
}

void
Server::doAccept() {
    this->_acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<Session>(std::move(socket), this)->start();
                } else {
                    std::cout << ec << std::endl;
                }

                this->doAccept();
            });
}

void
Server::deliver(const Message &msg, participant_ptr participant) {

    std::string tmp(msg.body());

    Command cmd(tmp.substr(0, msg.body_length()));

    this->_parser->execCommand(cmd, participant);
}

udp::endpoint &Server::getUdpEndpoint() {
    return (this->_udpEndPoint);
}

boost::asio::io_context &Server::getIo_context() {
    return io_context;
}
