//
// Created by kahoul on 10/11/18.
//

#include "../include/Session.hpp"
#include "../include/Server.hpp"

Session::Session(tcp::socket socket, Server *server)
        : _socket(std::move(socket)),
          _server(server) {
    std::cout << "Client Connected." << std::endl;
}

void
Session::start() {
    this->doReadHeader();
}

void
Session::deliver(const Message &msg) {
    bool writeInProgress = !_write_msgs.empty();
    _write_msgs.push_back(msg);
    if (!writeInProgress) {
        doWrite();
    }
}

void
Session::doReadHeader() {
    auto self(shared_from_this());
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_read_msg.data(), Message::header_length),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec && _read_msg.decode_header()) {
                                    this->doReadBody();
                                } else {
                                }
                            });
}

void
Session::doReadBody() {
    auto self(shared_from_this());
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec) {
                                    _server->deliver(_read_msg, shared_from_this());
                                    doReadHeader();
                                } else {
                                }
                            });
}

void
Session::doWrite() {
    auto self(shared_from_this());
    boost::asio::async_write(_socket,
                             boost::asio::buffer(_write_msgs.front().data(),
                                                 _write_msgs.front().length()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     _write_msgs.pop_front();
                                     if (!_write_msgs.empty()) {
                                         doWrite();
                                     }
                                 } else {
                                 }
                             });
}