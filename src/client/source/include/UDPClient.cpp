#include <iostream>
#include "UDPClient.hpp"
#include "Game.hpp"

UDPClient::UDPClient(boost::asio::io_context &io_context, const std::string &ip, int port, Game *game)
			:	_type(0) {
	_game = game;
	_receiverEndpoint = new udp::endpoint(address::from_string(ip), port);
	_socket = new udp::socket(io_context);
	_socket->open(udp::v4());
}

UDPClient::~UDPClient() {
	_socket->close();
	_updListenenerThread->join();
	delete(_socket);
	delete(_updListenenerThread);
	delete(_receiverEndpoint);
};

void UDPClient::startListener()
{
	/*_type = 0;
	_updListenenerThread = new std::thread([this]() {
		while (true) {
			_socket->async_receive_from(boost::asio::buffer(_listenerRecvBuffer), _listenerSenderEndpoint,
					boost::bind(&UDPClient::handleReceive, _listenerSenderEndpoint,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred));
		}
	});*/
}

void UDPClient::request(std::string msg, std::function<void(std::string)> callback) {

	_type = 1;
	boost::shared_ptr<std::string> message(new std::string(msg));


	_socket->async_send_to(boost::asio::buffer(*message), *_receiverEndpoint,
						   boost::bind(&UDPClient::handleSend, this, message,
									   boost::asio::placeholders::error,
									   boost::asio::placeholders::bytes_transferred));
	callback(_recvBuffer.data());
}

void UDPClient::handleReceive(const boost::system::error_code& error,
							  std::size_t bytes_transferred) {
	if (!error || error == boost::asio::error::message_size) {
		if (_type == 0) {
			_game->updateView(_listenerRecvBuffer.data());
		}
		else {
			std::cout << _recvBuffer.data() << std::endl;
		}
	}
}

void UDPClient::handleSend(boost::shared_ptr<std::string> message,
						   const boost::system::error_code& error,
						   std::size_t bytes_transferred) {
	if (!error || error == boost::asio::error::message_size) {
		_socket->async_receive_from(
				boost::asio::buffer(_recvBuffer), _senderEndpoint,
				boost::bind(&UDPClient::handleReceive, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
	}
}
