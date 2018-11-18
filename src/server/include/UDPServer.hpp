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

using boost::asio::ip::udp;

struct TestPlayer {
    int id;
    std::string name;
    int posX;
    int posY;
};

typedef std::string (*FncPtr)(std::vector<TestPlayer>);

class UDPParser {
public:
    UDPParser();
    ~UDPParser();

    void parseCommand(const std::string &, std::vector<TestPlayer>);

    std::string getCmdToSend();

private:
    static std::string getAllPlayerPositions(std::vector<TestPlayer>);

private:
    std::string _cmdToSend;
    std::map<std::string, FncPtr> _playerFncs;

};

//----------------------------------------------------------------------


class UDPServer {
public:
    UDPServer(boost::asio::io_context &, const udp::endpoint &);

    ~UDPServer();

private:
    void startReceive();

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_transferred);

    void handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& error,
                    std::size_t bytes_transferred);

private:
    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    boost::array<char, 1024> _recvBuffer;
    UDPParser _udpParser;
    std::vector<TestPlayer> _players;

};

#endif //R_TYPE_UDPSERVER_H
