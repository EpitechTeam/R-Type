//
// Created by kahoul on 17/11/18.
//

#include "../include/Server.hpp"
#include "../include/Parser.hpp"

void Parser::initPreGameCommands() {

    this->_functions[PreGameObj].emplace("CREATE_ROOM", make_pair(Parser::createRoom , 2 ));
    this->_functions[PreGameObj].emplace("JOIN_ROOM", make_pair(Parser::joinRoom, 1 ));
    this->_functions[PreGameObj].emplace("LEAVE_ROOM", make_pair(Parser::leaveRoom, 0 ));
    this->_functions[PreGameObj].emplace("SET_NAME", make_pair(Parser::setName, 1 ));
    this->_functions[PreGameObj].emplace("PRINT_ROOM", make_pair(Parser::printRoom, 1 ));
    this->_functions[PreGameObj].emplace("MESSAGE", make_pair(Parser::message, 1 ));
}

void
Parser::createRoom(Command &command, participant_ptr participant, Server *server) {
    std::string name(command.getArg(0));
    std::string slots(command.getArg(1));

    server->_rooms.emplace_back(server->getIo_context(), name, std::stoi(slots), server->getUdpEndpoint());
    Parser::writeResponse(participant, 200, "ROOM_CREATED");
}

void
Parser::joinRoom(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));


    auto tmp = Room::find(server->_rooms, roomName);
    if (tmp == NULL) {
        Parser::writeResponse(participant, 404 ,"ROOM_NOT_FOUND");
        std::cout << "Unknown room" << std::endl;
    } else {
        tmp->join(participant);
        std::cout << participant->getName() << " enter the room " << tmp->getName() << "." << std::endl;
        Parser::writeResponse(participant, 200, "ROOM_JOIN");
    }
}

void
Parser::leaveRoom(Command &command, participant_ptr participant, Server *server) {

    if (participant->_currentRoom) {

        std::cout << participant->getName() << " leave the room " << participant->_currentRoom->getName() << "." << std::endl;
        participant->_currentRoom->leave(participant);
        participant->_currentRoom = NULL;
        Parser::writeResponse(participant, 200, "ROOM_LEAVE");
    } else {
        std::cout << participant->getName() << " isn't in a room." << std::endl;
        Parser::writeResponse(participant, 400, "NOT_IN_ROOM");
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
        Parser::writeResponse(participant, 400, "NOT_IN_ROOM");
    }
}
