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


struct Response {
    int status;
    std::string message;
};

typedef std::function<Response(Command &, participant_ptr, Server *)> Function;

typedef std::pair<Function, std::size_t> Params;


class Parser {
public:
    Parser(Server *server);

    void initPreGameCommands();

    void execCommand(Command &command, participant_ptr participant);

private:
    static Response createRoom(Command &command, participant_ptr participant, Server *server);
    static Response joinRoom(Command &command, participant_ptr participant, Server *server);
    static Response leaveRoom(Command &command, participant_ptr participant, Server *server);
    static Response setName(Command &command, participant_ptr participant, Server *server);
    static Response message(Command &command, participant_ptr participant, Server *server);
    static Response getRooms(Command &command, participant_ptr participant, Server *server);
    static Response getRoomPlayers(Command &command, participant_ptr participant, Server *server);
    static Response getMessages(Command &command, participant_ptr participant, Server *server);
    static Response setReady(Command &command, participant_ptr participant, Server *server);
    static Response getReady(Command &command, participant_ptr participant, Server *server);

    static void writeResponse(participant_ptr participant, const Response &response);
    static void writeResponse(participant_ptr participant, Response &response);

    std::map<std::string, Params > _functions;
    Server *_server;
};

#endif //R_TYPE_PARSER_HPP
