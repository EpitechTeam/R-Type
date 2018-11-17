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
    this->_functions[PreGameObj].emplace("MESSAGE", make_pair(Parser::message, 1 ));
    this->_functions[PreGameObj].emplace("GET_ROOMS", make_pair(Parser::getRooms, 0 ));
    this->_functions[PreGameObj].emplace("GET_ROOM_PLAYERS", make_pair(Parser::getRoomPlayers, 1 ));
    this->_functions[PreGameObj].emplace("ROOM_STATE", make_pair(Parser::roomState, 1 ));
}

Response
Parser::createRoom(Command &command, participant_ptr participant, Server *server) {
    std::string name(command.getArg(0));
    std::string slots(command.getArg(1));

    server->_rooms.emplace_back(server->getIo_context(), name, std::stoi(slots), server->getUdpEndpoint());
    return { 200,  "ROOM_CREATED"};
}

Response
Parser::joinRoom(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));


    auto tmp = Room::find(server->_rooms, roomName);
    if (tmp == NULL) {
        std::cout << "Unknown room" << std::endl;
        return { 200,  "UNKNOWN_ROOM"};
    } else {
        tmp->join(participant);
        std::cout << participant->getName() << " enter the room " << tmp->getName() << "." << std::endl;
        return { 200, "ROOM_JOINED" };
    }
}

Response
Parser::leaveRoom(Command &command, participant_ptr participant, Server *server) {

    if (participant->_currentRoom) {

        std::cout << participant->getName() << " leave the room " << participant->_currentRoom->getName() << "." << std::endl;
        participant->_currentRoom->leave(participant);
        participant->_currentRoom = NULL;
        return { 200, "ROOM_LEAVE" };
    } else {
        std::cout << participant->getName() << " isn't in a room." << std::endl;
        return { 400, "NOT_IN_ROOM" };
    }
}

Response
Parser::setName(Command &command, participant_ptr participant, Server *server) {
    std::string name(command.getArg(0));
    participant->setName(name);
    std::cout << "Name changed to " << name << "." << std::endl;
    return { 200, "NAME_CHANGED" };
}

Response
Parser::message(Command &command, participant_ptr participant, Server *server) {
    std::string message(command.getArg(0));

    if (participant->_currentRoom) {

        Message msg(message);
        std::cout << "Message [";
        std::cout.write(msg.body(), msg.body_length());
        std::cout << "] sent to the room." << std::endl;
        participant->_currentRoom->deliver(msg);
        return { 200, "MESSAGE_DELIVER" };
    } else {
        std::cout << participant->getName() << " isn't in a room." << std::endl;
       return { 400, "NOT_IN_ROOM" };
    }
}

Response
Parser::getRooms(Command &command, participant_ptr participant, Server *server) {
    std::string response;

    if (server->_rooms.empty()) {
        return { 400, "NO_ROOMS" };
    }

    for (auto &it : server->_rooms) {
        response += it.getName() + " ";
    }
    return { 200, response };
}

Response
Parser::getRoomPlayers(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));
    std::string response;

    auto tmp = Room::find(server->_rooms, roomName);
    if (tmp == NULL) {
        std::cout << "Unknown room" << std::endl;
        return { 404 ,"ROOM_NOT_FOUND" };
    } else if (tmp->_participants.empty()) {
        return { 400, "No Players" };
    } else {
        for (auto &it : tmp->_participants) {
            response += it->getName() + " ";
        }
        return { 200, response };
    }
}

Response
Parser::roomState(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));

    auto tmp = Room::find(server->_rooms, roomName);
    if (tmp == NULL) {
        std::cout << "Unknown room" << std::endl;
        return { 404 ,"ROOM_NOT_FOUND" };
    } else if (tmp->_participants.empty()) {
        return { 400, "NO_PLAYERS" };
    } else {
        return { 200, "WAITING" };
    }
}