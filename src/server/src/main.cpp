/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include "../include/Server.hpp"

int main(int argc, char *argv[]) {
    try {
		int port = 4242;

        if (argc >= 2) {
			port = std::atoi(argv[1]);
		}

        boost::asio::io_context io_context;

        tcp::endpoint endpoint(tcp::v4(), port);
        udp::endpoint udpEndpoint(udp::v4(), port);
        Server server(io_context, endpoint, udpEndpoint);

        //UDPServer server(io_context, udpEndpoint);
        //Game game(io_context, udpEndpoint);
        io_context.run();
        //game.Start();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}