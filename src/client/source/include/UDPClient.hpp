#ifndef R_TYPE_UDPClient_H
#define R_TYPE_UDPClient_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <functional>
#include <string>

using boost::asio::ip::udp;
using boost::asio::ip::address;

class Game;

class UDPClient {
public:
	UDPClient(boost::asio::io_context &, const std::string& ip, int port, Game *game);
	~UDPClient();

	void request(std::string message, std::function<void(std::string)> callback);

	void startListener();
private:
	void handleReceive(const boost::system::error_code& error,
                              std::size_t bytes_transferred);

    void handleSend(boost::shared_ptr<std::string> message,
                    const boost::system::error_code& error,
                    std::size_t bytes_transferred);


private:
	udp::socket *_socket;
	udp::endpoint *_receiverEndpoint;
	udp::endpoint _senderEndpoint;
	udp::endpoint _listenerSenderEndpoint;
	char _recvBuffer[1024];
	char _listenerRecvBuffer[1024];
	std::thread *_updListenenerThread;
	Game *_game;
	int _type;
};


#endif