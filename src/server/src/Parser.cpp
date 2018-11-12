//
// Created by kahoul on 11/11/18.
//

#include "../include/Server.hpp"
#include "../include/Parser.hpp"

Parser::Parser(Server *server)
        : _server(server) {

    this->_functions.emplace("createRoom", Parser::createRoom);
    this->_functions.emplace("joinRoom", Parser::joinRoom);
    this->_functions.emplace("leaveRoom", Parser::leaveRoom);
    this->_functions.emplace("setName", Parser::setName);
    this->_functions.emplace("printRoom", Parser::printRoom);
    this->_functions.emplace("message", Parser::message);
}

void
Parser::execCommand(Command &command, participant_ptr participant) {

    auto function = this->_functions.find(command.getCommand());

    if (function != this->_functions.end()) {

        std::cout << "Command [" << command.getCommand() << "]:";

        for (int i = 0; i < command.argLen(); i++) {
            std::cout << command.getArg(i) << ":";
        }

        std::cout << " Executed." << std::endl;
        function->second(command, participant, this->_server);
    } else {
        std::cout << "Command [" << command.getCommand() << "] not Supported." << std::endl;
    }
}

void
Parser::createRoom(Command &command, participant_ptr participant, Server *server) {
    std::string name(command.getArg(0));
    std::string slots(command.getArg(1));

    server->_rooms.emplace_back(server->getIo_context(), name, std::stoi(slots), server->getUdpEndpoint());
}

void
Parser::joinRoom(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));


    auto tmp = Room::find(server->_rooms, roomName);
    if (tmp == NULL) {
        std::cout << "Unknown room" << std::endl;
    } else {
        tmp->join(participant);
        std::cout << participant->getName() << " enter the room " << tmp->getName() << "." << std::endl;
    }
}

void
Parser::leaveRoom(Command &command, participant_ptr participant, Server *server) {

    if (participant->_currentRoom) {

        std::cout << participant->getName() << " leave the room " << participant->_currentRoom->getName() << "." << std::endl;
        participant->_currentRoom->leave(participant);
        participant->_currentRoom = NULL;
    } else {
        std::cout << participant->getName() << " isn't in a room." << std::endl;
    }
}

void
Parser::setName(Command &command, participant_ptr participant, Server *server) {
    std::string name(command.getArg(0));
    participant->setName(name);
    std::cout << "Name changed to " << name << "." << std::endl;
}

void
Parser::printRoom(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));

    auto tmp = Room::find(server->_rooms, roomName);
    std::cout << "Room " << roomName << "[";
    if (tmp->_participants.size() > 0) {
        for (auto it : tmp->_participants) {
            std::cout << it->getName() << ", ";
        }
    }

    std::cout << "]" << std::endl;
}

void
Parser::message(Command &command, participant_ptr participant, Server *server) {
    std::string message(command.getArg(0));

    if (participant->_currentRoom) {

        Message msg(message);
            std::cout << "Message [";
            std::cout.write(msg.body(), msg.body_length());
            std::cout << "] sent to the room." << std::endl;
            participant->_currentRoom->deliver(msg);
    } else {
        std::cout << participant->getName() << " isn't in a room." << std::endl;
    }
}