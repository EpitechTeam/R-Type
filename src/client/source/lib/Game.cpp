//
// Created by cherkaoui on 16/11/18.
//

#include "Game.hpp"
#include "RequestManager.hpp"
#include "RType.hpp"

Game::Game(RType *rType) : rType(rType) {
    if (!texture.loadFromFile("myasset/background.png")) {
        std::cout << "ERROR TEXTURE" << std::endl;
    }
    background = sf::Sprite(texture);
    background.setScale(1.5f, 1.5f);

    if (!font.loadFromFile("myasset/old.ttf")) {
        std::cout << "ERROR FONT" << std::endl;
    }
    text.setFont(font);
    text.setString("Game name:");
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setPosition(20, 650);

    front_promt.setFont(font);
    front_promt.setCharacterSize(30);
    front_promt.setPosition(10, 250);
    front_promt.setFillColor(sf::Color::White);

    starship.push_back(new Starship(this));
};

void Game::init_udp()
{
    client = new UDPClient(rType->io_context, rType->ip, std::stoi(rType->port), this);

    udpclientIoThread = new std::thread([this]() {
        rType->io_context.run();
        std::cout << "udp disconnected : " << std::endl;
        this->rType->view = AUTH;
    });

    client->request("INIT_PLAYER " + rType->auth->playername, [this](std::string cmd) {
        std::cout << "connection udp: "<< cmd << std::endl;
    });
}

