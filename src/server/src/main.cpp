/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include "../include/Server.hpp"

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
        udp::endpoint udpEndpoint(udp::v4(), std::atoi(argv[1]));
        //Server server(io_context, endpoint, udpEndpoint);

        //UDPServer server(io_context, udpEndpoint);
        Game game(io_context, udpEndpoint);
//        io_context.run();
        game.Start();


    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}