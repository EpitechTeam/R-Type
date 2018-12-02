//
// Created by David Zakrzewski on 17/11/2018.
//

#include <iostream>
#include <string>
#include <UDPServer.hpp>

#include "UDPGame.hpp"

UDPParser::UDPParser() {
    _playerFncs["GET_POSITIONS"] = UDPParser::getAllPositions;
    _playerFncs["DEAD"] = UDPParser::killEntity;
    _playerFncs["FIRE_BULLET"] = UDPParser::fireBullet;
    _playerFncs["INIT_PLAYER"] = UDPParser::initPlayer;
    _playerFncs["UPDATE_SCORE"] = UDPParser::updateScore;
    _playerFncs["GET_SCORE"] = UDPParser::getScore;
    _playerFncs["MSG"] = UDPParser::sendMessageToAll;
    _playerFncs["READY"] = UDPParser::playerReady;
    _playerFncs["MOVE_PLAYER"] = UDPParser::movePlayer;
    _playerFncs["COLLISION"] = UDPParser::collision;

}
UDPParser::~UDPParser() = default;

// ######################    ROUTES   ###########################
std::string UDPParser::getAllPositions(UDPGame *game, UDPServer *server) {
    std::ostringstream ss;

    ss << "GET_POSITIONS ";
    for(auto player: (game->GetPlayers())) {
        if (player.GetLife() == 0)
            continue;
        ss << player.GetPosition().x << ":" << player.GetPosition().y;
        ss << ":" << player.GetId() << ":-1:" << player.GetAsset();
        ss << " ";
    }

    int monsterAsset;

    for(auto monster: (game->GetMonsters())) {
        if (monster.GetLife() == 0 || monster.isSpawned() == false)
            continue;
        if (monster.GetType() == "normal") {
            monsterAsset = 1;
        }
        if (monster.GetType() == "runner") {
            monsterAsset = 2;
        }
        ss << monster.GetPosition().x << ":" << monster.GetPosition().y;
        ss << ":-1:" << monster.GetId() << ":" << monsterAsset;
        ss << " ";
    }
    ss << std::endl;
    return (ss.str());
}

std::string UDPParser::killEntity(UDPGame *game, UDPServer *server) {

    std::string id = server->GetCommand()->at(1);
    std::cout << "kill monster id: " << id << std::endl;
    game->KillMonster(id);
    server->SendToAll("DEAD " + id);
    return ("200");
}

std::string UDPParser::fireBullet(UDPGame *game, UDPServer *server) {
    //game->CreateBullet(std::stoi(server->GetCommand()->at(1)), std::stoi(server->GetCommand()->at(2)), "player");
    server->NewBullet(std::stoi(server->GetCommand()->at(1)), std::stoi(server->GetCommand()->at(2)), "player");
    return ("200");
}

std::string UDPParser::initPlayer(UDPGame *game, UDPServer *server) {
    server->AddClient(server->GetCommand()->at(1), server->GetRemoteEndpoint());
    return ("200");
}

std::string UDPParser::updateScore(UDPGame *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);
    player->SetScore(std::stoi(server->GetCommand()->at(1)));
    return ("200");
}

std::string UDPParser::getScore(UDPGame *game, UDPServer *server) {
    int score = 0;
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);
    score = player->GetScore();
    return ("200 " + std::to_string(score));
}

std::string UDPParser::sendMessageToAll(UDPGame *game, UDPServer *server) {
    server->SendToAll(server->GetCommand()->at(1));
    return ("200");
}

std::string UDPParser::playerReady(UDPGame *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);

    player->SetReady(std::stoi(server->GetCommand()->at(1)));
    return ("200");
}

std::string UDPParser::movePlayer(UDPGame *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);

    player->SetPosition({std::stod(server->GetCommand()->at(1)), std::stod(server->GetCommand()->at(2))});
    return ("200");
}

std::string UDPParser::collision(UDPGame *game, UDPServer *server) {
    Client client = server->GetClientByRemotepoint(server->GetRemoteEndpoint());
    Player *player = server->GetPlayerByClient(client);

    if (player->GetLife() > 0)
        player->SetLife(player->GetLife() - 1);
    return ("200");
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

void UDPParser::parseCommand(UDPGame *game, UDPServer *server) {
    if (server->GetCommand()->size() == 0) {
        _cmdToSend = "404";
        return;
    }
    std::string cmd = server->GetCommand()->at(0);
    if (_playerFncs.count(cmd) == 0) {
        _cmdToSend = "404";
        return;
    }
    _cmdToSend = _playerFncs[cmd](game, server);
}

std::string UDPParser::getCmdToSend() {
    return (_cmdToSend);
}

UDPServer::UDPServer(udp::socket socket, UDPGame *game)
        : _socket(std::move(socket)), _game(game){

    startReceive();
}

UDPServer::~UDPServer() {
    delete(_command);
    delete(_game);
};

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
        //std::string newCmd = command.substr(0, command.length() - 1);

        _command = new UDPCommand(split(command, " "));

        _udpParser.parseCommand(_game, this);

        //std::cout << "UDP receive: " << _recvBuffer.data() << "\n";

        auto message = std::make_shared<std::string>(_udpParser.getCmdToSend());
        _socket.async_send_to(boost::asio::buffer(*message), _remoteEndpoint,
                              boost::bind(&UDPServer::handleSend, this, message,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
        _recvBuffer.assign(0);
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
        std::cout << name + " already exists\n";
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

void UDPServer::NewBullet(double x, double y, std::string owner) {
    SendToAll("NEW_BULLET " + std::to_string(x) + " " + std::to_string(y) + " " + owner);
}

