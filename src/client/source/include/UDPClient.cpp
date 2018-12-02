#include <iostream>
#include "UDPClient.hpp"
#include "Game.hpp"

UDPClient::UDPClient(boost::asio::io_context &io_context, const std::string &ip, int port, Game *game)
			:	_type(0) {
	_game = game;
	_receiverEndpoint = new udp::endpoint(address::from_string(ip), port);
	_socket = new udp::socket(io_context);
	_socket->open(udp::v4());
	//std::cout << "listener started\n";
	_updListenenerThread = new std::thread([this]() {
		startListener();
	});
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
	_type = 0;

	_socket->async_receive_from(boost::asio::buffer(_listenerRecvBuffer),
			_listenerSenderEndpoint, [this](const boost::system::error_code& ec,
											std::size_t len) {
		_game->updateView(std::string(_listenerRecvBuffer));
		memset(_listenerRecvBuffer, 0, sizeof(_listenerRecvBuffer));
		startListener();
	});
}

void UDPClient::request(std::string msg, std::function<void(std::string)> callback) {

	_type = 1;
	boost::shared_ptr<std::string> message(new std::string(msg));


	_socket->async_send_to(boost::asio::buffer(*message), *_receiverEndpoint,
                           boost::bind(&UDPClient::handleSend, this, message,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
}

void UDPClient::handleSend(boost::shared_ptr<std::string> message,
						   const boost::system::error_code& error,
						   std::size_t bytes_transferred) {
	if (!error || error == boost::asio::error::message_size) {
	}
}
