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

class ScopeLock {
public:
    explicit ScopeLock(std::mutex &mutex)
        : mtx(&mutex)
    {
        this->mtx->lock();
    }

    ~ScopeLock() {
        this->mtx->unlock();
    }

private:
    std::mutex *mtx;
};

struct Request {
    Message request;
    std::function<void(Command &command)> callback;
};

class RequestManager {
private:

    bool requestExec() {
        ScopeLock lock(mtx);

        if (!this->_requests.empty()) {
            Request *request = &this->_requests.front();

            this->_rType->_client->write(request->request);

            Message msg = this->_rType->_client->waitingForResponse();
            std::string tmp(msg.body());
            Command cmd(tmp.substr(0, msg.body_length()));
            request->callback(cmd);

            this->_requests.pop_front();
        }
        return true;
    }

public:

    RequestManager(RType &rType)
            : _rType(&rType) {
        this->_thread = new std::thread([this]() {
            while (this->requestExec());
        });
    }

    ~RequestManager() {
        if (this->_thread) {
            this->_thread->join();
        }
    }

    void request(const std::string &request, std::function<void(Command &command)> callback) {
        ScopeLock lock(this->mtx);

        this->_requests.push_back({Message{request}, std::move(callback)});
    }

    void request(Message &request, std::function<void(Command &command)> callback) {
        ScopeLock lock(this->mtx);

        this->_requests.push_back({request, std::move(callback)});
    }

private:
    std::thread *_thread;
    RType *_rType;
    std::deque<Request> _requests;
    std::mutex mtx;
};

#endif //R_TYPE_REQUEST_HPP
