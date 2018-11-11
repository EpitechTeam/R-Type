//
// Created by kahoul on 10/11/18.
//

#include "../include/Server.hpp"

Server::Server
(boost::asio::io_context &io_context, const tcp::endpoint &endpoint)
        : _acceptor(io_context, endpoint) {
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
                }

                this->doAccept();
            });
}

void
Server::deliver(const Message &msg, participant_ptr participant) {

    std::string tmp(msg.body());

    Command cmd(tmp.substr(0, msg.body_length()));

    std::cout << "[" << cmd.getCommand() << "]:";

    for (int i = 0; i < cmd.argLen(); i++) {
        std::cout << cmd.getArg(i) << ":";
    }

    std::cout << std::endl;

    this->execCommand(cmd, participant);
}

void
Server::execCommand(Command &command, participant_ptr participant) {

    if (command.getCommand() == "newRoom") {
        std::string name(command.getArg(0));
        std::string slots(command.getArg(1));

        this->_rooms.emplace_back(name, std::stoi(slots));
    }
    if (command.getCommand() == "joinRoom") {
        std::string roomName(command.getArg(0));


        auto tmp = Room::find(this->_rooms, roomName);
        if (tmp == NULL) {
            std::cout << "Unknown room" << std::endl;
        } else {
            tmp->join(participant);
            std::cout << participant->getName() << " enter the room" << tmp->getName() << "." << std::endl;
        }
    }
    if (command.getCommand() == "setName") {
        std::string name(command.getArg(0));
        participant->setName(name);
        std::cout << "Name changed to " << name << "." << std::endl;
    }
    if (command.getCommand() == "printRoom") {
        std::string roomName(command.getArg(0));

        auto tmp = Room::find(this->_rooms, roomName);
        std::cout << "Room " << roomName << "[";
        for (auto it : tmp->_participants) {
            std::cout << it->getName() << ", ";
        }
        std::cout << "]" << std::endl;
    }
}