//
// Created by kahoul on 10/11/18.
//

#include <Room.hpp>

#include "../include/Room.hpp"

Room::Room(boost::asio::io_context &io_context, std::string &name, int maxSlots, const udp::endpoint& udpEndpoint)
        : _name(name), _maxSlots(maxSlots), _udpServer(io_context, udpEndpoint) {
    std::cout << "Room " << name << " with " << maxSlots << " slots max created." << std::endl;
}

void
Room::join(participant_ptr participant) {
    _participants.insert(participant);
    for (auto msg: _recent_msgs)
        participant->deliver(msg);
}

void
Room::leave(participant_ptr participant) {
    _participants.erase(participant);
}

void
Room::deliver(const Message &msg) {
    _recent_msgs.push_back(msg);
    while (_recent_msgs.size() > max_recent_msgs)
        _recent_msgs.pop_front();

    for (auto participant: _participants)
        participant->deliver(msg);
}

UDPServer::UDPServer(boost::asio::io_context& io_context, const udp::endpoint &endpoint)
            : _socket(io_context, endpoint){
    startReceive();
}

UDPServer::~UDPServer() = default;

void UDPServer::startReceive() {
    _socket.async_receive_from(boost::asio::buffer(_recvBuffer),
            _remoteEndpoint, boost::bind(&UDPServer::handleReceive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handleReceive(const boost::system::error_code &error, size_t bytes_transferred) {
    auto message = std::make_shared<std::string>("Hello world\n");

    _socket.async_send_to(boost::asio::buffer(*message), _remoteEndpoint,
            boost::bind(&UDPServer::handleSend, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handleSend(std::shared_ptr<std::string> message, const boost::system::error_code &ec,
                           std::size_t bytes_transferred) {
    startReceive();
}
