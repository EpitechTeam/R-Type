//
// Created by kahoul on 11/11/18.
//

#include "../include/Server.hpp"
#include "../include/Parser.hpp"

Parser::Parser(Server *server)
        : _server(server) {

    this->initPreGameCommands();
    // this->initGameCommands();
}

void
Parser::execCommand(Command &command, participant_ptr participant) {

    auto function = this->_functions.find(command.getCommand());

    if (function != this->_functions.end()) {

        std::cout << "Command [" << command.getCommand() << "]:";

        for (unsigned int i = 0; i < command.argLen(); i++) {
            std::cout << command.getArg(i) << ":";
        }

        std::cout << " Executed." << std::endl;
        if (command.getCommand() == "MESSAGE" || command.argLen() == function->second.second) {
            Response response = function->second.first(command, participant, this->_server);
            Parser::writeResponse(participant, response);
        } else {
            std::cout << "Bad arguments nb: expects "<< function->second.second << " arguments, " << command.argLen()
            << " provided." << std::endl;
            Parser::writeResponse(participant, { 400, "BAD_ARGUMENTS_NUMBERS" });

        }
    } else {
        std::cout << "Command [" << command.getCommand() << "] not Supported." << std::endl;
        Parser::writeResponse(participant, { 400, "COMMAND_NOT_SUPPORTED" });
    }
}

void
Parser::writeResponse(participant_ptr participant, const Response &response) {
 participant->deliver(Message(std::to_string(response.status) + " " + response.message));
}
void
Parser::writeResponse(participant_ptr participant, Response &response) {
    participant->deliver(Message(std::to_string(response.status) + " " + response.message));
}
