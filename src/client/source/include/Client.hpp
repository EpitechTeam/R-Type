/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        Client.hpp
*/

#ifndef PROJECT_CLIENT_HPP
#define PROJECT_CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#define  MENU 1
#define  LOBBY 2
#define  CREATEROOM 3
#define  JOINROOM 4
#define  ROOM 5
#define AUTH 6
#define  MAP 7

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<Message> MessageQueue;

class Client {
public:
    Client(boost::asio::io_context &ioContext,
           const tcp::resolver::results_type &endpoints)
            : _ioContext(ioContext),
              _socket(ioContext) {
        do_connect(endpoints);
    }

    void write(const Message &msg) {
        boost::asio::post(_ioContext,
                          [this, msg]() {
                              bool write_in_progress = !_writeMsgs.empty();
                              _writeMsgs.push_back(msg);
                              if (!write_in_progress) {
                                  do_write();
                              }
                          });
    }

    void close() {
        boost::asio::post(_ioContext, [this]() { _socket.close(); });
    }

    bool responseAvailable() {
        return (!this->_readMsgs.empty());
    }

    Message getResponse() {

        if (!this->_readMsgs.empty()) {
            Message tmp = this->_readMsgs.front();
            this->_readMsgs.pop_front();
            return (tmp);
        } else {
            return Message{"404 NO_RESPONSE"};
        }
    }

    Message waitingForResponse() {
        while (!this->responseAvailable());
        return this->getResponse();
    }

private:

    void do_connect(const tcp::resolver::results_type &endpoints) {
        boost::asio::async_connect(_socket, endpoints,
                                   [this](boost::system::error_code ec, tcp::endpoint) {
                                       if (!ec) {
                                           do_read_header();
                                       }
                                   });
    }

    void do_read_header() {
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_readMsg.data(), Message::header_length),
                                [this](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec && _readMsg.decode_header()) {
                                        do_read_body();
                                    } else {
                                        std::cout << "ERROR: Socket close." << std::endl;
                                        _socket.close();
                                    }
                                });
    }

    void do_read_body() {
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_readMsg.body(), _readMsg.body_length()),
                                [this](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec) {
                                        this->_readMsgs.emplace_back(this->_readMsg);
                                        do_read_header();
                                    } else {
                                        std::cout << "ERROR: Socket close." << std::endl;
                                        _socket.close();
                                    }
                                });
    }

    void do_write() {
        boost::asio::async_write(_socket,
                                 boost::asio::buffer(_writeMsgs.front().data(),
                                                     _writeMsgs.front().length()),
                                 [this](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         _writeMsgs.pop_front();
                                         if (!_writeMsgs.empty()) {
                                             do_write();
                                         }
                                     } else {
                                         std::cout << "ERROR: Socket close." << std::endl;
                                         _socket.close();
                                     }
                                 });
    }

private:
    boost::asio::io_context &_ioContext;
    tcp::socket _socket;
    Message _readMsg;
    MessageQueue _readMsgs;
    MessageQueue _writeMsgs;
};

sf::Sprite createSprite(const std::string path);

void print(std::string string);

int roll(int min, int max);

#endif //PROJECT_CLIENT_HPP
