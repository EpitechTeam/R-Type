//
// Created by David Zakrzewski on 17/11/2018.
//

#ifndef R_TYPE_UDPSERVER_H
#define R_TYPE_UDPSERVER_H

#include <vector>
#include <string>
#include <map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include "Command.hpp"
#include "Entities.hpp"

class UDPGame;
class UDPServer;
using boost::asio::ip::udp;

std::vector<std::string> split(std::string phrase, std::string delimiter);

typedef std::string (*FncPtr)(UDPGame *game, UDPServer *server);

class UDPParser {
public:
    UDPParser();
    ~UDPParser();

    void parseCommand(UDPGame *game, UDPServer *server);

    std::string getCmdToSend();

    static std::string getAllPositions(UDPGame *game, UDPServer *server);
    static std::string killEntity(UDPGame *game, UDPServer *server);
    static std::string fireBullet(UDPGame *game, UDPServer *server);
    static std::string initPlayer(UDPGame *game, UDPServer *server);
    static std::string updateScore(UDPGame *game, UDPServer *server);
    static std::string getScore(UDPGame *game, UDPServer *server);
    static std::string getAllScore(UDPGame *game, UDPServer *server);
    static std::string sendMessageToAll(UDPGame *game, UDPServer *server);
    static std::string playerReady(UDPGame *game, UDPServer *server);
    static std::string movePlayer(UDPGame *game, UDPServer *server);
    static std::string collision(UDPGame *game, UDPServer *server);

private:
    std::string _cmdToSend;
    std::map<std::string, FncPtr> _playerFncs;

};

//----------------------------------------------------------------------

typedef std::vector<std::pair<std::string, udp::endpoint>> ClientList;
typedef std::pair<std::string, udp::endpoint> Client;
typedef std::vector<std::string> UDPCommand;

class UDPServer {
public:
    UDPServer(udp::socket socket, UDPGame *game);
    ~UDPServer();

    void SendTo(std::string msg, udp::endpoint ep);
    void SendToAll(std::string msg);

    UDPCommand *GetCommand();
    udp::endpoint GetRemoteEndpoint();

    void AddClient(std::string name, udp::endpoint endpoint);

    ClientList &GetClients();

    std::pair<std::string, udp::endpoint> GetClientByRemotepoint(udp::endpoint ep);

    Player *GetPlayerByClient(Client& client);

    void NewBullet(double x, double y, std::string owner);

private:

    void startReceive();

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_transferred);

    void handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& error,
                    std::size_t bytes_transferred);

    bool IsInVector(std::pair<std::string, udp::endpoint> pair);


private:
    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    boost::array<char, 1024> _recvBuffer;
    UDPParser _udpParser;
    ClientList _clients;
    UDPCommand *_command;
    UDPGame *_game;
};

#endif //R_TYPE_UDPSERVER_H
