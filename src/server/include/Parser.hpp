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

typedef std::function<void(Command &, participant_ptr, Server *)> Function;

typedef std::pair<Function, int> Params;

class Parser {
public:
    Parser(Server *server);

    void initPreGameCommands();

    void initGameCommands();

    void execCommand(Command &command, participant_ptr participant);

private:
    static void createRoom(Command &command, participant_ptr participant, Server *server);
    static void joinRoom(Command &command, participant_ptr participant, Server *server);
    static void leaveRoom(Command &command, participant_ptr participant, Server *server);
    static void setName(Command &command, participant_ptr participant, Server *server);
    static void printRoom(Command &command, participant_ptr participant, Server *server);
    static void message(Command &command, participant_ptr participant, Server *server);

    static void writeResponse(participant_ptr participant, int status, std::string msg);

    std::map<Type, std::map<std::string, Params >>_functions;
    Server *_server;
};

#endif //R_TYPE_PARSER_HPP
