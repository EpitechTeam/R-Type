#include <iostream>
#include "UDPClient.hpp"

UDPClient::UDPClient(boost::asio::io_context &io_context, const std::string &ip) {
	_receiverEndpoint = new udp::endpoint(address::from_string(ip), 4242);
	_socket = new udp::socket(io_context);
	_socket->open(udp::v4());
}

UDPClient::~UDPClient() {
	_socket->close();
	delete(_socket);
	delete(_receiverEndpoint);
};

void UDPClient::request(std::string message, std::function<void(std::string)> callback) {
	_socket->send_to(boost::asio::buffer(message), *_receiverEndpoint);

	_socket->receive_from(boost::asio::buffer(_recvBuffer), _senderEndpoint/*, boost::bind(&UDPClient::handleReceive, this,
                                                            boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred)*/);
	callback(_recvBuffer.data());
}

void UDPClient::handleReceive(const boost::system::error_code& error,
                              std::size_t bytes_transferred) {
    	if (!error || error == boost::asio::error::message_size) {
    	}
}