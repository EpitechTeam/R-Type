/*
** EPITECH PROJECT, 2018
** R-Type
** File description:
**        main.cpp
*/

#include "RType.hpp"
#include "RequestManager.hpp"
#include "Client.hpp"

int main(int argc, char **argv) {

    RType rType;

    while (rType.window->isOpen()) {
        sf::Event event;
        while (rType.window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                rType.window->close();
            rType.event(event);
        }
        rType.window->clear();
        rType.draw();
        rType.window->display();
    }
    return 0;
}