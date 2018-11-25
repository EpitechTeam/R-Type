#ifndef R_TYPE_UDPClient_H
#define R_TYPE_UDPClient_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <functional>
#include <string>

using boost::asio::ip::udp;
using boost::asio::ip::address;

class UDPClient {
public:
	UDPClient(boost::asio::io_context &, const std::string& ip);
	~UDPClient();

	void request(std::string message, std::function<void(std::string)> callback);
private:
	void handleReceive(const boost::system::error_code& error,
                              std::size_t bytes_transferred);

private:
	udp::socket *_socket;
	udp::endpoint *_receiverEndpoint;
	udp::endpoint _senderEndpoint;
	boost::array<char, 1024> _recvBuffer;
};


#endif