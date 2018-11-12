//
// Created by kahoul on 11/11/18.
//

#include "../include/Server.hpp"
#include "../include/Parser.hpp"

Parser::Parser(Server *server)
        : _server(server) {

    this->_functions.emplace("createRoom", make_pair(Parser::createRoom , 2 ));
    this->_functions.emplace("joinRoom", make_pair(Parser::joinRoom, 1 ));
    this->_functions.emplace("leaveRoom", make_pair(Parser::leaveRoom, 0 ));
    this->_functions.emplace("setName", make_pair(Parser::setName, 1 ));
    this->_functions.emplace("printRoom", make_pair(Parser::printRoom, 1 ));
    this->_functions.emplace("message", make_pair(Parser::message, 1 ));
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
        if (command.argLen() == function->second.second) {
            function->second.first(command, participant, this->_server);
        } else {
            std::cout << "Bad arguments nb: expects "<< function->second.second << " arguments, " << command.argLen()
            << " provided." << std::endl;
        }
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