//
// Created by David Zakrzewski on 17/11/2018.
//

#include <iostream>
#include <UDPServer.hpp>

#include "UDPServer.hpp"

UDPParser::UDPParser() {
    _playerFncs["getAllPlayersPositions"] = UDPParser::getAllPlayerPositions;

}
UDPParser::~UDPParser() {

}

std::string UDPParser::getAllPlayerPositions(std::vector<TestPlayer> Players) {
    std::ostringstream ss;

    for(auto const& player: Players) {
        ss << player.name << "," << player.posX << "," << player.posY << ";";
    }
    ss << std::endl;
    return (ss.str());
}

void UDPParser::parseCommand(const std::string &cmd, std::vector<TestPlayer> Players) {
    if (_playerFncs.count(cmd) == 0) {
        _cmdToSend = "\n";
        return;
    }
    _cmdToSend = _playerFncs[cmd](Players);
}

std::string UDPParser::getCmdToSend() {
    return (_cmdToSend);
}

UDPServer::UDPServer(boost::asio::io_context& io_context, const udp::endpoint &endpoint, Game *game)
        : _socket(io_context, endpoint), _game(game){
    _players.push_back({0, "player1", 10, 20});
    _players.push_back({1, "player2", 5, 13});
    startReceive();
}

UDPServer::~UDPServer() {

}

void UDPServer::Test() {
    std::cout << "Testing server\n";
}

void UDPServer::startReceive() {
    _socket.async_receive_from(boost::asio::buffer(_recvBuffer),
                               _remoteEndpoint, boost::bind(&UDPServer::handleReceive, this,
                                                            boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handleReceive(const boost::system::error_code& error,
                              std::size_t bytes_transferred) {
    if (!error || error == boost::asio::error::message_size) {
        std::string command = std::string(_recvBuffer.begin(), _recvBuffer.begin()+bytes_transferred);
        std::string newCmd = command.substr(0, command.length() - 1);
        std::cout << "# command: " << newCmd << "#" << std::endl;

        _udpParser.parseCommand(newCmd, _players);

        auto message = std::make_shared<std::string>(_udpParser.getCmdToSend());

        _socket.async_send_to(boost::asio::buffer(*message), _remoteEndpoint,
                              boost::bind(&UDPServer::handleSend, this, message,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));

        startReceive();
    }
}

void UDPServer::handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& error,
                           std::size_t bytes_transferred) {
}
