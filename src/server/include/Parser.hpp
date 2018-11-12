//
// Created by kahoul on 11/11/18.
//

#ifndef R_TYPE_PARSER_HPP
#define R_TYPE_PARSER_HPP

#include <map>
#include <functional>

#include "Command.hpp"
#include "Participant.hpp"

class Server;

class Parser {
public:
    Parser(Server *server);

    void execCommand(Command &command, participant_ptr participant);

private:
    static void createRoom(Command &command, participant_ptr participant, Server *server);
    static void joinRoom(Command &command, participant_ptr participant, Server *server);
    static void leaveRoom(Command &command, participant_ptr participant, Server *server);
    static void setName(Command &command, participant_ptr participant, Server *server);
    static void printRoom(Command &command, participant_ptr participant, Server *server);
    static void message(Command &command, participant_ptr participant, Server *server);

    std::map<std::string,
    std::function<void(Command &, participant_ptr, Server *server)>> _functions;
    Server *_server;
};

#endif //R_TYPE_PARSER_HPP
