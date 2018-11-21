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
            : _rType(&rType) {
    }

    ~RequestManager() {
        for (auto &it : this->_threads) {
            it.join();
        }
    }

    void request(Message &command, std::function<void(Command &command)> callback) {
        this->_threads.emplace_back([this, command, &callback]() {
            // this->_client->do_read_header();
            this->_rType->_client->write(command);
            Message msg = this->_rType->_client->waitingForResponse();
            std::string tmp(msg.body());
            Command cmd(tmp.substr(0, msg.body_length()));
            callback(cmd);
            //Message msg(this->_client->do_read_header());
            //std::cout <<  "Message = ";
            //std::cout.write(msg.body(), msg.body_length());
            //std::cout << "\n";
        });
    }

private:
    RType *_rType;
    std::vector<std::thread> _threads;
};

#endif //R_TYPE_REQUEST_HPP
