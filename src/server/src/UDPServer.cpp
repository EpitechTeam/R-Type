//
// Created by David Zakrzewski on 17/11/2018.
//

#include <iostream>
#include <string>
#include <UDPServer.hpp>

#include "Game.hpp"

UDPParser::UDPParser() {
    _playerFncs["GET_POSITIONS"] = UDPParser::getAllPositions;
    _playerFncs["DEAD"] = UDPParser::killEntity;
    _playerFncs["FIRE_BULLET"] = UDPParser::fireBullet;
    _playerFncs["INIT_PLAYER"] = UDPParser::initPlayer;
    _playerFncs["UPDATE_SCORE"] = UDPParser::updateScore;
    _playerFncs["GET_SCORE"] = UDPParser::getScore;
    _playerFncs["MSG"] = UDPParser::sendMessageToAll; // Todo handle messages with spaces
    _playerFncs["READY"] = UDPParser::playerReady;
    _playerFncs["MOVE_PLAYER"] = UDPParser::movePlayer;
    _playerFncs["COLLISION"] = UDPParser::collision;

}
UDPParser::~UDPParser() = default;

// ######################    ROUTES   ###########################
std::string UDPParser::getAllPositions(Game *game, UDPServer *server) {
    std::ostringstream ss;

    ss << "200 ";
    for(auto player: (game->GetPlayers())) {
        if (player.GetLife() == 0)
            continue;
        ss << player.GetPosition().x << ":" << player.GetPosition().y << ":1:";
        ss << "0:" << player.GetId() << ":-1:" << player.GetAsset();
    }
    ss << " ";
    for(auto monster: (game->GetMonsters())) {
        if (monster.GetLife() == 0)
            continue;
        ss << monster.GetPosition().x << ":" << monster.GetPosition().y << ":-1:";
        ss << "0:" << "-1:" << monster.GetId() << ":lool.png";
    }

    ss << " ";
    for(auto bullet: (game->GetBullets())) {
        ss << bullet.GetPosition().x << ":" << bullet.GetPosition().y << ":-1:"; // Todo: change this "-1"
        ss << "1:" << "-1:" << "-1:" << "lool.png";
    }
    ss << std::endl;
    return (ss.str());
}

std::string UDPParser::killEntity(Game *game, UDPServer *server) {

    return ("200\n");
}

std::string UDPParser::fireBullet(Game *game, UDPServer *server) {
    game->CreateBullet(std::stoi(server->GetCommand()->at(1)), std::stoi(server->GetCommand()->at(2)), std::stoi(server->GetCommand()->at(3)));
    return ("200\n");
}

std::string UDPParser::initPlayer(Game *game, UDPServer *server) {
    server->AddClient(server->GetCommand()->at(1), server->GetRemoteEndpoint());
    return ("200\n");
}

std::string UDPParser::updateScore(Game *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);
    player->SetScore(std::stoi(server->GetCommand()->at(1)));
    return ("200\n");
}

std::string UDPParser::getScore(Game *game, UDPServer *server) {
    int score = 0;
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);
    score = player->GetScore();
    return ("200 " + std::to_string(score) + "\n");
}

std::string UDPParser::sendMessageToAll(Game *game, UDPServer *server) {
    server->SendToAll(server->GetCommand()->at(1));
    return ("200\n");
}

std::string UDPParser::playerReady(Game *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);

    player->SetReady(std::stoi(server->GetCommand()->at(1)));
    return ("200\n");
}

std::string UDPParser::movePlayer(Game *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);

    player->SetPosition({std::stoi(server->GetCommand().at(1)), std::stoi(server->GetCommand().at(2))})
    return ("200\n");
}

std::string UDPParser::collision(Game *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);

    if (player->GetLife() > 0)
        player->SetLife(player->GetLife() - 1);
    return ("200\n");
}

// #####################################################################

Player *UDPServer::GetPlayerByClient(Client& client) {
    Player *player = NULL;
    auto it = find_if(_game->GetPlayers().begin(), _game->GetPlayers().end(), [&client](Player player) {
        return (client.first == player.GetId());
    });

    if (it != _game->GetPlayers().end()) {
        auto index = std::distance(_game->GetPlayers().begin(), it);
        player = &(_game->GetPlayers()[index]);
    }
    return (player);
}

std::pair<std::string, udp::endpoint> UDPServer::GetClientByRemotepoint(udp::endpoint ep) {
    std::pair<std::string, udp::endpoint> pair;

    for (auto client : _clients) {
        if (client.second == ep)
            return (client);
    }
    return (pair);
}

void UDPParser::parseCommand(Game *game, UDPServer *server) {
    if (server->GetCommand()->size() == 0) {
        _cmdToSend = "404\n";
        return;
    }
    std::string cmd = server->GetCommand()->at(0);
    std::cout << cmd + "\n";
    if (_playerFncs.count(cmd) == 0) {
        _cmdToSend = "404\n";
        return;
    }
    _cmdToSend = _playerFncs[cmd](game, server);
}

std::string UDPParser::getCmdToSend() {
    return (_cmdToSend);
}

UDPServer::UDPServer(boost::asio::io_context& io_context, const udp::endpoint &endpoint, Game *game)
        : _socket(io_context, endpoint), _game(game){

    startReceive();
}

UDPServer::~UDPServer() = default;

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

        _command = new UDPCommand(split(newCmd, " "));

        _udpParser.parseCommand(_game, this);

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

/*
 * Check if client already exists by @name et @endpoint
 */
bool UDPServer::IsInVector(std::pair<std::string, udp::endpoint> pair) {
    for (auto client : _clients) {
        if (client.second == pair.second || client.first == pair.first) {
            return (true);
        }
    }
    return (false);
}

/*
 * Returns command in cache
 */
UDPCommand *UDPServer::GetCommand() {
    return (_command);
}

/*
 * Returns Remotepoint in cache
 */
udp::endpoint UDPServer::GetRemoteEndpoint() {
    return (_remoteEndpoint);
}

/*
 * Room need to use it to add clients
 */
void UDPServer::AddClient(std::string name, udp::endpoint endpoint) {
    auto pair = std::make_pair(name, endpoint);

    if (!IsInVector(pair)) {
        _clients.push_back(std::pair<std::string, udp::endpoint>(name, endpoint));
        _game->AddPlayer(name);
        std::cout << name + " joined the game\n";
    }
    else
        std::cout << name + " already exists" << std::endl;
}

/*
 * Send a @message to @endpoint
 */
void UDPServer::SendTo(std::string msg, udp::endpoint ep) {
    _socket.send_to(boost::asio::buffer(msg), ep);
}

/*
 * Send a @message to all
 */
void UDPServer::SendToAll(std::string msg) {
    for (auto client : _clients) {
        _socket.send_to(boost::asio::buffer(msg), client.second);
    }
}

/*
 * Returns all clients
 */
ClientList &UDPServer::GetClients() {
    return (_clients);
}

