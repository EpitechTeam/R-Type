//
// Created by kahoul on 10/11/18.
//

#include "../include/Room.hpp"

Room::Room(boost::asio::io_context &io_context, std::string &name, int maxSlots, const udp::endpoint& udpEndpoint)
        : _name(name), _maxSlots(maxSlots), _udpServer(io_context, udpEndpoint) {
    std::cout << "Room " << name << " with " << maxSlots << " slots max created." << std::endl;
}

void
Room::join(participant_ptr participant) {
    _participants.insert(participant);
    participant->setRoom(this);
    for (auto msg: _recent_msgs)
        participant->deliver(msg);
}

void
Room::leave(participant_ptr participant) {
    std::cout << participant->getName() << " leave the room " << this->getName() << "." << std::endl;
    participant->setRoom(NULL);
    _participants.erase(participant);
}

void
Room::deliver(const Message &msg) {
    _recent_msgs.push_back(msg);
   /* while (_recent_msgs.size() > max_recent_msgs)
        _recent_msgs.pop_front();

    for (auto participant: _participants)
        participant->deliver(msg);*/
}

std::string
Room::getMessages() {
    std::string tmp;

    for (auto it : this->_recent_msgs) {
        tmp.append(it.body(), it.body_length());
        tmp.push_back('|');
    }
    return tmp;
}

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

UDPServer::UDPServer(boost::asio::io_context& io_context, const udp::endpoint &endpoint)
            : _socket(io_context, endpoint){
    _players.push_back({0, "player1", 10, 20});
    _players.push_back({1, "player2", 5, 13});
    startReceive();
}

UDPServer::~UDPServer() {

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
