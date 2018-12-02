//
// Created by kahoul on 17/11/18.
//

#include "../include/Server.hpp"
#include "../include/Parser.hpp"

void Parser::initPreGameCommands() {

    this->_functions.emplace("CREATE_ROOM", make_pair(Parser::createRoom , 2 ));
    this->_functions.emplace("JOIN_ROOM", make_pair(Parser::joinRoom, 1 ));
    this->_functions.emplace("LEAVE_ROOM", make_pair(Parser::leaveRoom, 0 ));
    this->_functions.emplace("SET_NAME", make_pair(Parser::setName, 1 ));
    this->_functions.emplace("MESSAGE", make_pair(Parser::message, 1 ));
    this->_functions.emplace("GET_ROOMS", make_pair(Parser::getRooms, 0 ));
    this->_functions.emplace("GET_ROOM_PLAYERS", make_pair(Parser::getRoomPlayers, 1 ));
    this->_functions.emplace("GET_MESSAGES", make_pair(Parser::getMessages, 0 ));
    this->_functions.emplace("SET_READY", make_pair(Parser::setReady, 0 ));
    this->_functions.emplace("GET_READY", make_pair(Parser::getReady, 0 ));
}

Response
Parser::createRoom(Command &command, participant_ptr participant, Server *server) {
    std::string name(command.getArg(0));
    std::string slots(command.getArg(1));

    auto tmp = Room::find(server->_rooms, name);

    if (tmp == NULL) {
        server->_rooms.emplace_back(server->getIo_context(), name, std::stoi(slots), server->getUdpEndpoint());
        return { 200,  "ROOM_CREATED"};
    } else {
        return { 400,  "ROOM_ALREADY_EXIST"};
    }
}

Response
Parser::joinRoom(Command &command, participant_ptr participant, Server *server) {
    std::string roomName(command.getArg(0));


    auto tmp = Room::find(server->_rooms, roomName);
    if (tmp == NULL) {
        std::cout << "Unknown room" << std::endl;
        return { 400,  "UNKNOWN_ROOM"};
    } else if (tmp->_game.isGameStarted()) {
        return { 400,  "GAME_ALREADY_STARTED"};
    } else if (tmp->_maxSlots <= tmp->_participants.size()) {
        return { 400,  "ROOM_FULL"};
    } else {
        tmp->join(participant);
        return { 200, std::to_string(tmp->_maxSlots) };
    }
}

Response
Parser::leaveRoom(Command &command, participant_ptr participant, Server *server) {

    if (participant->_currentRoom) {

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
    std::string message;

    for (std::size_t i = 0; i < command.argLen(); i++ ) {
        message.append(command.getArg(i));
        message.push_back(' ');
    }

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
Parser::getMessages(Command &command, participant_ptr participant, Server *server) {

    if (participant->_currentRoom) {
        return { 200, participant->_currentRoom->getMessages() };
    } else {
        return { 400, "NOT_IN_ROOM" };
    }
}

Response
Parser::setReady(Command &command, participant_ptr participant, Server *server) {

    if (participant->_currentRoom) {
        participant->setReady();

        if (participant->_currentRoom->isAllPlayerReady()) {
            std::cout << "GAME READY" << std::endl;

            participant->_currentRoom->_game.Start();

            // participant->_currentRoom->_game->start();
        }
        return { 200, participant->getReady() ? " True" : "False" };
    } else {
        return { 400, "NOT_IN_ROOM" };
    }
}

Response
Parser::getReady(Command &command, participant_ptr participant, Server *server) {
    std::string response;

    if (participant->_currentRoom) {

        for (auto &it : participant->_currentRoom->_participants) {
            response += it->getName() + (it->getReady() ?  ": READY" : ": NOT READY") + "|";
        }
        return { 200, response };
    } else {
        return { 400, "NOT_IN_ROOM" };
    }
}