#include <utility>

//
// Created by kahoul on 19/11/18.
//

#ifndef R_TYPE_REQUEST_HPP
#define R_TYPE_REQUEST_HPP

#include <functional>
#include <thread>
#include <mutex>
#include <deque>

#include "Command.hpp"
#include "Message.hpp"
#include "RType.hpp"

#include "ScopeLock.hpp"

struct Request {
    Message request;
    std::function<void(Command &command)> callback;
};

class RequestManager {
private:

    bool requestExec() {
        if (!this->_requests.empty()) {
            ScopeLock lock(mtx);

            Request *request = &this->_requests.front();

            this->_rType->client->write(request->request);

            Message msg = this->_rType->client->waitingForResponse();
            std::string tmp(msg.body());
            Command cmd(tmp.substr(0, msg.body_length()));
            request->callback(cmd);

            this->_requests.pop_front();
        }
        return true;
    }

public:

    RequestManager(RType *rType)
            : _rType(rType) {
        this->_thread = new std::thread([this]() {
            while (this->requestExec());
        });
    }

    ~RequestManager() {
        if (this->_thread) {
            this->_thread->join();
        }
    }

    bool isDuplicate(Message request) {
        for (auto it : this->_requests) {
            if (it.request.body_length() == request.body_length() && !strcmp(it.request.body(), request.body()))
                return true;
        }
        return false;
    }

    void printRequests() {
        std::cout << "Requests: " << std::endl;
        for (auto it : this->_requests) {
            std::cout.write(it.request.body(), it.request.body_length());
            std::cout << std::endl;
        }
    }

    void request(const std::string &request, std::function<void(Command &command)> callback) {
        ScopeLock lock(this->mtx);

        Message msg(request);

        if (!this->isDuplicate(msg)) {
            this->_requests.push_back({Message{request}, callback});
        }
    }

    void request(Message &request, std::function<void(Command &command)> callback) {
        ScopeLock lock(this->mtx);

        if (!this->isDuplicate(request)) {
            this->_requests.push_back({request, callback});
        }
    }

private:
    std::thread *_thread;
    RType *_rType;
    std::deque<Request> _requests;
    std::mutex mtx;
};

#endif //R_TYPE_REQUEST_HPP
