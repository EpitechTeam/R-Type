//
// Created by kahoul on 19/11/18.
//

#ifndef R_TYPE_REQUEST_HPP
#define R_TYPE_REQUEST_HPP

#include <functional>
#include <thread>
#include "Command.hpp"
#include "Message.hpp"
#include "RType.hpp"

class RequestManager {
public:

    RequestManager(RType &rType)
        : _rType(&rType){
    }

    RequestManager(Client &client)
            : _client(&client){
    }

    ~RequestManager() {
        for (auto &it : this->_threads) {
            it.join();
        }
    }

    void request(Message &command/*, std::function<void(Command &command)> callback*/) {
        this->_threads.emplace_back([this, command]() {
           // this->_client->do_read_header();
            this->_client->write(command);
            Message *tmp = this->_client->waitingForResponse();
            if (tmp && tmp->body() != command.body()) {
                std::cout << "Response: ";
                std::cout.write(tmp->body(), tmp->body_length());
                std::cout << "\n";
            } else {
                std::cout << "Error: No Response." << std::endl;
            }
            //Message msg(this->_client->do_read_header());
            //std::cout <<  "Message = ";
            //std::cout.write(msg.body(), msg.body_length());
            //std::cout << "\n";
        });
    }

private:
    RType *_rType;
    Client *_client;
    std::vector<std::thread> _threads;
};

#endif //R_TYPE_REQUEST_HPP
