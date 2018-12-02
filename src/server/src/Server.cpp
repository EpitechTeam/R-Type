//
// Created by kahoul on 10/11/18.
//

#include "../include/Server.hpp"

const int NUM_SECONDS = 5;

Server::Server(const int port)
        : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)),
          _parser(new Parser(this)) {
    std::cout << "Tcp Server listening to " << port << std::endl << std::endl;
    /*this->_updateT = new std::thread([this]() {

        while (true) {
            this->deleteEmptyRooms();
        }
    });*/
    doAccept();
}

Server::~Server() {
    if (this->_updateT) {
        this->_updateT->join();
    }
}

void Server::run() {
    try {
        this->io_context.run();
    }
    catch (std::exception &e) {
        std::cerr << "Tcp Server: " << e.what() << "\n";
    }
}

Room *Server::roomFind(std::string &name) {
    this->roomMutex.lock();

    for (auto &i : this->_rooms) {
        if (i.getName() == name) {
            this->roomMutex.unlock();
            return &i;
        }
    }

    this->roomMutex.unlock();
    return (NULL);
}

void Server::RoomAdd(std::string &name, int maxSlots) {
    this->roomMutex.lock();

    this->_rooms.emplace_back(name, maxSlots);
    this->roomMutex.unlock();
}

void Server::deleteEmptyRooms() {
    this->roomMutex.lock();

    for (auto it = this->_rooms.begin(); it != this->_rooms.end(); ++it) {
        if (!it->_participants.empty()) {
            it->_timeout = 0;
        } else if (it->_timeout == 0) {
            it->_timeout = clock();
        }
        if (it->_timeout != 0 && (double) (clock() - it->_timeout) >= (double) (NUM_SECONDS * CLOCKS_PER_SEC)) {
            std::cout << "Room " << it->getName() << " removed for inactivity." << std::endl;
            this->_rooms.erase(it);
        }
    }
    this->roomMutex.unlock();
}

void
Server::doAccept() {
    this->_acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
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

boost::asio::io_context &Server::getIo_context() {
    return io_context;
}
