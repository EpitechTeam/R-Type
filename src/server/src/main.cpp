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

        Server server(port);

        server.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}